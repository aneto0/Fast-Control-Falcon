#/bin/bash
echo "Going to repeat experiment for $1 pulses"
for i in `seq 1 $1`
do
    echo "Starting new experiment. Sending OPENTREE"
    curl "http://localhost:8084/BROWSE/StateMachine?StatusChangeRequest=OPENTREE"
    echo "Waiting"
    sleep 2
    echo "Sending ACQUIRE"
    curl "http://localhost:8084/BROWSE/StateMachine?StatusChangeRequest=ACQUIRE"
    sleep 10
    echo "Waiting"
    echo "Sending STOP"
    curl "http://localhost:8084/BROWSE/StateMachine?StatusChangeRequest=STOP"
done
