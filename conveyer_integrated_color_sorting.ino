//assumption clockwise theta to be positive
//run test 1 changes required:
//for this we will need to introduce a previous color variable and need to caliberate the sensing output to show a particular color over the belt
//can caliberate the sensor to show values at a range as black and whenever the color is not black it moves the belt accordingly
//this way the belt will stop the object at the sensor
//the color caliberation for black over complete belt is required

#define S0_PIN 12 
#define S1_PIN 13
#define S2_PIN 10
#define S3_PIN 11
#define OUT_PIN  9
int enA = 3;
int in1 = 4;
int in2 = 5;
int enB = 6;
int in3 = 8;
int in4 = 7;
int blue=0;  //assigning any arbitrary scale to map to the colors(the values were based on a random scale and were obtained after running the motor to detect different colors)
int red=255;
int green=-255;
float theta=0;

void rotate(float* theta, int color);
void clockwise(float* theta, int color);
void counterclockwise(float* theta, int color);
void brake(float* theta, int color);

void rotate(float* theta,int color){  //the rotate function will decide on the basis of color and theta regarding how much more the motor has to rotate
  if (color<*theta){
    clockwise(theta,color);
  }
  else if (color>*theta){
    counterclockwise(theta,color);
  }
  else{
    brake(theta,color);
  }
}

void brake(float* theta, int color){
  digitalWrite(in1, LOW); //USED TO BRAKE THE FLOW
  digitalWrite(in2, LOW); 
}

void clockwise(float* theta,int color) {
  while(*theta>(color+26)){
    int startTime=millis();
    digitalWrite(in1, HIGH); // Direction for Motor A
    digitalWrite(in2, LOW);  // Direction for Motor A
    delay(100);
    int currentTime=millis();
    int time=currentTime-startTime;
    Serial.print(time);
    *theta-=time*0.255;
    Serial.println();
    Serial.print(*theta);
    Serial.println();
  }
  brake(theta,color);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  delay(3000);//after detecting a particular color and moving according to it we have stopped the motor A , giving the block time to come and fall into it.
}

void counterclockwise(float* theta,int color) {
  while(*theta<(color-26)){
    int startTime=millis();  //used to get an idea of how much the motor has moved
    digitalWrite(in1, LOW); // Direction for Motor A
    digitalWrite(in2, HIGH);  // Direction for Motor A
    delay(100);
    int currentTime=millis();
    int time=currentTime-startTime;
    Serial.print(time);
    *theta+=time*0.255;
    Serial.println();
    Serial.print(*theta);
    Serial.println();
  }
  brake(theta,color);
  delay(3000);
  loop();
}

void setup() {
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Set OUT_PIN as Input
  pinMode(OUT_PIN, INPUT);
  // Set Pulse Width scaling to 20%
  digitalWrite(S0_PIN, HIGH);
  digitalWrite(S1_PIN, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  //motor speed settings
  analogWrite(enA, 255); // Speed for Motor A (0-255)
  analogWrite(enB, 255); // Speed for Motor B (0-255)
  digitalWrite(in3, HIGH); // Direction for Motor B
  digitalWrite(in4, LOW);  // Direction for Motor B
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  int r, g, b;
  r = process_red_value();
  g = process_green_value();
  b = process_blue_value();
  Serial.print("r = ");
  Serial.print(r);
  Serial.print(" ");
  Serial.print("g = ");
  Serial.print(g);
  Serial.print(" ");
  Serial.print("b = ");
  Serial.print(b);
  Serial.print(" ");
  Serial.println();

  if ((r>150)&&(b>150)&&(g>150)){
    Serial.print('black');
  }
  else{
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);     //stops the belt as soon as the color changes from black
    if (r<g && r<b){
    Serial.print("red ");
    rotate(&theta,red);
    }
    else if (g<r && g<b){
      Serial.print("green ");
      rotate(&theta,green);
    }
    else{
      Serial.print("blue ");
      rotate(&theta,blue);
    }
    Serial.print("");
    }
}

int process_red_value() {
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, LOW);
  int pulse_length = pulseIn(OUT_PIN, LOW);
  return pulse_length;
}

int process_green_value() {
  digitalWrite(S2_PIN, HIGH);
  digitalWrite(S3_PIN, HIGH);
  int pulse_length = pulseIn(OUT_PIN, LOW);
  return pulse_length;
}

int process_blue_value() {
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, HIGH);
  int pulse_length = pulseIn(OUT_PIN, LOW);
  return pulse_length;
}
