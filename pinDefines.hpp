#define echoFor 14
#define echoRgh 15
#define echoLft 16
#define trigPin 17

void declarePins(){
    pinMode(trigPin, OUTPUT);
    pinMode(echoFor, INPUT);
    pinMode(echoRgh, INPUT);
    pinMode(echoLft, INPUT);
}
