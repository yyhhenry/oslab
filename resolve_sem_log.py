# Semaphore Log Format:
# <Obj> <id> <action>
# <Obj>: Producer | Consumer
# <id>: <int>
# <action>: starts ... | exits ... | pushes <int> | gets <int> from Producer <int>


queue: list[int] = []
cnt_consumer: dict[int, int] = {}
numbering: dict[tuple[int, int], int] = {}
numbering_cnt: int = 0
result: list[str] = []


def get_numbering(producer_id: int, value: int) -> int:
    global numbering_cnt
    if (producer_id, value) not in numbering:
        numbering_cnt += 1
        numbering[(producer_id, value)] = numbering_cnt
    return numbering[(producer_id, value)]


with open("exp4/sem.log", "r") as f:
    lines = f.readlines()
    for line in lines:
        parts = [x.strip() for x in line.split()]
        if len(parts) < 3:
            continue

        obj = parts[0].lower()
        id = int(parts[1])
        action = parts[2]
        if action in ["starts", "exits"]:
            continue
        if obj == "producer" and action == "pushes":
            value = int(parts[3])
            key = get_numbering(id, value)
            queue.append(key)
            result.append(f"Producer {id+1} pushes {key} (turn {value+1})\n")
            result.append(f"{queue=}\n\n")
        elif obj == "consumer" and action == "gets":
            # Consumer <consumer_id> gets <value> from Producer <producer_id>
            # 0        1             2    3       4    5        6
            value = int(parts[3])
            producer_id = int(parts[6])
            key = get_numbering(producer_id, value)
            queue.remove(key)
            if id not in cnt_consumer:
                cnt_consumer[id] = 0
            cnt_consumer[id] += 1
            result.append(f"Consumer {id+1} gets {key} (turn {cnt_consumer[id]})\n")
            result.append(f"{queue=}\n\n")
        else:
            raise Exception(f"Unknown object: {obj}")


with open("exp4/sem_res.log", "w") as f:
    f.writelines(result)
