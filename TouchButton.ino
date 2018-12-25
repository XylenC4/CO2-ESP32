int iMenu = 0;
int iMenuCount = 5;

bool bTouchPlus, bTouchMinus, bTouchSet;
bool bTouchPlus_OLD, bTouchMinus_OLD, bTouchSet_OLD;

void setup_Touch() {
  pinMode(TOUCH_PLUS, INPUT);       
  pinMode(TOUCH_MINUS, INPUT);       
  pinMode(TOUCH_SET, INPUT);         
}

void loop_Touch() {
  bTouchPlus = digitalRead(TOUCH_PLUS);
  bTouchMinus = digitalRead(TOUCH_MINUS);
  bTouchSet = digitalRead(TOUCH_SET);

 if(bTouchPlus != bTouchPlus_OLD && bTouchPlus) {
  iMenu += 1;
  CheckMenuOrder();
  //OledDisplayLogAdd("Touch +", 500);
 }
 
 if(bTouchMinus != bTouchMinus_OLD && bTouchMinus) {
  iMenu -= 1;
  CheckMenuOrder();
  //OledDisplayLogAdd("Touch -", 500);
 }
  
 if(bTouchSet != bTouchSet_OLD && bTouchSet) {
  //OledDisplayLogAdd("Touch SET", 500);
 }
   
  bTouchPlus_OLD = bTouchPlus;
  bTouchMinus_OLD = bTouchMinus;
  bTouchSet_OLD = bTouchSet;
}

int CheckMenuOrder() {
    if(iMenu < 0) {
    iMenu = iMenuCount;
  }
  else if (iMenu > iMenuCount) {
    iMenu = 0;
  }
}
