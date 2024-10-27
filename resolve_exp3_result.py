import argparse

arg_parser = argparse.ArgumentParser()
arg_parser.add_argument(
    "input", type=str, help="Input file containing the output of the experiment"
)

parsed_args = arg_parser.parse_args()

input_file = str(parsed_args.input)
print(input_file)

parent_pid = 0
child_pids = []

# *** Input file format ***
# * There may be extra lines in the file, ignore them
# Parent PID <pid>
parent_stmt_pattern = "Parent PID "
# Child PIDs <pid> <pid> <pid> <pid> <pid> ...
childs_stmt_pattern = "Child PIDs "

with open(input_file, "r") as f:
    lines = f.readlines()
    for line in lines:
        if line.startswith(parent_stmt_pattern):
            parent_pid = int(line[len(parent_stmt_pattern) :])
        elif line.startswith(childs_stmt_pattern):
            child_pids = list(map(int, line[len(childs_stmt_pattern) :].split()))

print(f"Parent PID: {parent_pid}, Child PIDs: {child_pids}")

# *** Input file format Part 2 ***
# Jiffies <time>, PID <pid>, <action>
# <action> = <state> | '->' <state> | '->' <state> (<description>) | 'Forked from' <pid>
# <state> = 'Ready' | 'Running' | 'Waiting' | 'Exited'
# * We only care about the <state> field, ignore the "Forked from" action


class ProcessStatus:
    def __init__(self, pid: int, state: str, jiffies: int):
        self.pid = pid
        self.state = state
        self.jiffies = jiffies
        self.wait_time = 0
        self.run_time = 0

    def update_time(self, jiffies: int, state: str):
        assert state in ["Ready", "Running", "Waiting", "Exited"]
        assert (
            self.jiffies <= jiffies or self.state == "Exited"
        ), f"Time should be increasing, but got {self.pid}: {self.jiffies} -> {jiffies}"
        if self.state in ["Ready", "Wait"]:
            self.wait_time += jiffies - self.jiffies
        elif self.state == "Running":
            self.run_time += jiffies - self.jiffies
        self.state = state
        self.jiffies = jiffies


processes: dict[int, ProcessStatus] = {}

start_time = -1
end_time = 0

with open(input_file, "r") as f:
    lines = f.readlines()
    for line in lines:
        parts = [x.strip() for x in line.split(",")]
        if len(parts) < 3:
            continue
        if not parts[0].startswith("Jiffies"):
            continue
        jiffies = int(parts[0].split(" ")[1])
        if not parts[1].startswith("PID"):
            continue
        pid = int(parts[1].split(" ")[1])
        if pid == parent_pid:
            if parts[2].split(" ")[0] == "Ready":
                start_time = jiffies
            if parts[2].split(" ")[0] == "Exited":
                end_time = jiffies
                break
            continue
        if pid not in child_pids:
            continue
        if parts[2].startswith("Forked from"):
            continue
        action_parts = parts[2].split(" ")
        state = action_parts[0]
        if state == "->":
            state = action_parts[1]

        if pid not in processes:
            processes[pid] = ProcessStatus(pid, state, jiffies)
        else:
            processes[pid].update_time(jiffies, state)

sum_wait_time = 0
sum_run_time = 0
count = 0

for (pid, process) in processes.items():
    process.update_time(end_time, "Exited")
    wait_time = process.wait_time
    run_time = process.run_time
    print(f"PID: {pid}, Wait Time: {wait_time}, Run Time: {run_time}")
    
    sum_wait_time += wait_time
    sum_run_time += run_time
    count += 1
        
avg_wait_time = sum_wait_time / count
avg_run_time = sum_run_time / count


def jiffies_to_seconds(jiffies: float | int) -> float:
    return jiffies / 100.0


avg_wait_time_sec = jiffies_to_seconds(avg_wait_time)
avg_run_time_sec = jiffies_to_seconds(avg_run_time)
total_time_sec = jiffies_to_seconds(end_time - start_time)

print(
    f"Average Wait Time: {avg_wait_time_sec:.3}s, Average Run Time: {avg_run_time_sec:.3}s"
)

print(f"Throughput: {count * 60 / total_time_sec:.6} sub_task/min")
