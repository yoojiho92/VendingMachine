#include<Servo.h>
Servo servo;

#define DEBUG 1

/***********************
 * 각 센서의 핀 선언 시작
 ***********************/
#define INFRARED_SENSOR A0  // 적외선 센서 A0핀

const int MICRO_ECHO_1 = 4; // 50원 동전 인식 초음파 센서 Echo D4핀
const int MICRO_TRIG_1 = 5; // 50원 동전 인식 초음파 센서 Trig D5핀

const int MICRO_ECHO_2 = 6; // 100원 동전 인식 초음파 센서 Echo D6핀
const int MICRO_TRIG_2 = 7; // 100원 동전 인식 초음파 센서 Trig D7핀

const int MICRO_ECHO_3 = 8; // 500원 동전 인식 초음파 센서 Echo D8핀
const int MICRO_TRIG_3 = 9; // 500원 동전 인식 초음파 센서 Trig D9핀

const int PIEZO = 10;       // 피에조 D10핀

const int SERVO_PIN = 11;   // 서보 모터 D11핀

const int ButtonPin = 12;   // 푸시 버튼 D12핀
/**********************
 * 각 센서의 핀 선언 끝
 **********************/

/*********************
 * 동전 변수 선언 시작
 *********************/
int coin_50 = 0;      // 투입된 50원 짜리 동전 수          
int coin_100 = 0;     // 투입된 100원 짜리 동전 수
int coin_500 = 0;     // 투입된 500원 짜리 동전 수
int coin_total = 0;   // 투입된 동전의 합산 금액

const int COIN_50_LIMIT = 3;    // 50원 초음파 센서 인식거리 (단위,cm)
const int COIN_100_LIMIT = 3;   // 100원 초음파 센서 인식거리 (단위,cm) 
const int COIN_500_LIMIT = 3;   // 500원 초음파 센서 인식거리 (단위,cm) 
/********************
 *  동전 변수 선언 끝
 ********************/

int main_distance = 0;  // 적외선 센서의 거리 변수
int servo_value;        // 서보모터의 현재 각도 변수
int time_count = 0;     // 시간 카운트 변수
const int PRODUCT_PRICE = 500;// 상품의 가격 변수


/*****************
 *  Debug 문자열 선언 시작
 *****************/
String insert_50_message = "50원짜리 동전이 투입되었습니다.";
String insert_100_message = "100원짜리 동전이 투입 되었습니다.";
String insert_500_message = "500원짜리 동전이 투입 되었습니다.";
String insert_sound_message = "투입 소리 출력.";
String push_sound_message = "버튼 눌림 소리 출력";
String coin_init_message = "돈이 초기화 되었습니다.";
String push_button_message = "버튼이 눌렸습니다.";
String serbo_run_message = "서보 모터가 작동 합니다.";
String short_of_coins_message = "돈이 부족합니다.";
String commodity_discharge_message = "상품이 배출 됩니다.";
/************************
 *  Debug 문자열 선언 끝
 ************************/


/*******************
 * 함수 선 선언 시작
 *******************/
void Insert_Sound();                // 동전 투입 소리 출력
void Push_Sound();                  // 버튼 눌림 소리 출력
void Micro_1();                     // 50원 짜리 동전 투입 확인( 초음파 센서 1 )
void Micro_2();                     // 100원 짜리 동전 투입 확인( 초음파 센서 2 )
void Micro_3();                     // 500원 짜리 동전 투입 확인( 초음파 센서 3 )
void Coin_init();                   // 코인 초기화
void Coin_sum();                    // 투입되어진 코인 수를 계산하여 돈으로 환산 후 coin_total 변수에 저장
void Infrared_distance();           // 적외선 센서의 데이터를 받아와 cm로 변환 후 main_distance 변수에 저장
void servo_run(int dgree);          // 서보 모터를 해당위치에서 dgree위치로 이동
void Push_button();                 // 푸시 버튼이 눌렸을 시 서보 모터 작동
void Debug_Message(String message); // Debug가 1일 경우, 디버그 메시지 출력
/*****************
 * 함수 선 선언 끝
 *****************/
 
void Insert_Sound(){
  digitalWrite(PIEZO, HIGH);
  delay(100);
  digitalWrite(PIEZO, LOW);
  time_count = 1;
  Debug_Message(insert_sound_message);
}

void Push_Sound(){
  digitalWrite(PIEZO, HIGH);
  delay(1000);
  digitalWrite(PIEZO, LOW);
  delay(100);
  digitalWrite(PIEZO, HIGH);
  delay(1000);
  digitalWrite(PIEZO, LOW);
  time_count = 1;
  Debug_Message(push_sound_message);
}

void Micro_1(){
  digitalWrite(MICRO_TRIG_1, LOW);
  digitalWrite(MICRO_ECHO_1, LOW);
  delayMicroseconds(2);
  digitalWrite(MICRO_TRIG_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(MICRO_TRIG_1, LOW);
 
  unsigned long duration = pulseIn(MICRO_ECHO_1, HIGH);
  float distance = duration / 29.0 / 2.0;
  if (distance < COIN_50_LIMIT) {
    coin_50 += 1;
    Insert_Sound();
    Coin_sum();
    Debug_Message(insert_50_message);
  }
}

void Micro_2(){
  digitalWrite(MICRO_TRIG_2, LOW);
  digitalWrite(MICRO_ECHO_2, LOW);
  delayMicroseconds(2);
  digitalWrite(MICRO_TRIG_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(MICRO_TRIG_2, LOW);
 
  unsigned long duration = pulseIn(MICRO_ECHO_2, HIGH);
  float distance = duration / 29.0 / 2.0;
  if (distance < COIN_100_LIMIT) {
    coin_100 += 1;
    Insert_Sound();
    Coin_sum();
    Debug_Message(insert_100_message);
  }
}

void Micro_3(){
  digitalWrite(MICRO_TRIG_3, LOW);
  digitalWrite(MICRO_ECHO_3, LOW);
  delayMicroseconds(2);
  digitalWrite(MICRO_TRIG_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(MICRO_TRIG_3, LOW);
 
  unsigned long duration = pulseIn(MICRO_ECHO_3, HIGH);
  float distance = duration / 29.0 / 2.0;
  if (distance < COIN_500_LIMIT) {
    coin_500 += 1;
    Insert_Sound();
    Coin_sum();
    Debug_Message(insert_500_message);
  }
}

void Coin_init(){
  coin_50 = 0;
  coin_100 = 0;
  coin_500 = 0;
  coin_total = 0;
}

void Coin_sum(){
  int coin_total = coin_50 * 50 + coin_100 * 100 + coin_500 * 500;
  if(DEBUG){
    Serial.println("-------------");
    Serial.print(coin_total);
    Serial.println("의 금액이 있습니다.");
    Serial.println("-------------");
  }
  
}

void Infrared_distance(){
  // 거리값을 cm로 변환해주기 쉽게 하기위해서 map함수를 사용해서 데이터 값을 변환후
  int volt = map(analogRead(INFRARED_SENSOR), 0, 1023, 0, 5000);
  
  // 거리값을 cm변환하는 계산공식
  main_distance = (27.61 / (volt - 0.1696)) * 1000;
}

void servo_run(int dgree){
  int difference = servo_value - dgree;
  if(difference < 0){
    for(int angle = servo_value; angle < servo_value + dgree; angle++) 
    { 
      servo_value = angle;
      servo.write(angle); 
      delay(15); 
    } 
  }else if(difference > 0){
    for(int angle = servo_value; angle < servo_value - dgree; angle--) 
    { 
      servo_value = angle;
      servo.write(angle); 
      delay(15); 
    } 
  }
  Debug_Message(serbo_run_message);
}

void Push_button(){
  if (digitalRead(ButtonPin) == LOW)
  {
    if(coin_total > 500 || coin_total == 500){
      coin_total -= PRODUCT_PRICE;
      if(servo_value > 150){
        servo_run(0);
      }else {
        servo_run(servo_value + 30);
      }
      Debug_Message(commodity_discharge_message);
    }else {
      Debug_Message(short_of_coins_message);
    }
    Debug_Message(push_button_message);
  }
}

void Debug_Message(String message){
  if(DEBUG){
    Serial.println(message);
  }
}


void setup() {
  Serial.begin(9600);

  servo.attach(SERVO_PIN);
  pinMode(MICRO_TRIG_1, OUTPUT);
  pinMode(MICRO_ECHO_1, INPUT);
  pinMode(MICRO_TRIG_2, OUTPUT);
  pinMode(MICRO_ECHO_2, INPUT);
  pinMode(MICRO_TRIG_3, OUTPUT);
  pinMode(MICRO_ECHO_3, INPUT);
  pinMode(PIEZO, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);
}

void loop() {
  Infrared_distance();   // 적외선 센서 거리인식
  while(main_distance < 30){  // 적외선 센서에 인식된 거리가 30cm 이하 일 경우
    boolean piezo_flag = true;    // 초음파 센서 플래그 선언  
    if(time_count == 0 || time_count % 2000 == 0){   // 2초 간격을 피에조를 통해 소리를 출력
      // piezo_flag == true > HIGH, 
      // piezo_flag == flase > LOW
      if(piezo_flag){         
        piezo_flag = false;
        digitalWrite(PIEZO, HIGH);
      }else {
        piezo_flag = true;
        digitalWrite(PIEZO, LOW);
      }
    }

    Micro_1();      // 50원 짜리 동전 인식 초음파 센서 실행
    Micro_2();      // 100원 짜리 동전 인식 초음파 센서 실행
    Micro_3();      // 500원 짜리 동전 인식 초음파 센서 실행
    Push_button();  // 버튼 눌림여부 판단
    time_count += 1;// 시간 변수 ++
    delay(1);       // 1/1000초 대기
  }
  time_count = 0;
}
