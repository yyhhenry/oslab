gcc consumer.c -o consumer
gcc producer.c -o producer
echo "Starting producer"
producer &
echo "You need to run the consumer manually"
