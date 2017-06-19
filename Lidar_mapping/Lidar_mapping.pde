 import processing.serial.*;

float pcolor = 0; // point color value
float xVal = 0;  // x value
float yVal = 0;  // y value

Serial myPort;

void setup() {
  
  size(400,200);
  
  printArray(Serial.list());  // lista svih slobodnih serijskih portova
  
  myPort = new Serial(this, Serial.list()[0], 57600);  // otvaranje porta koji se koristi pri baud-rate od 57600
  
  myPort.bufferUntil('\n');   // ne generirat serialEvent() ako nismo dobili znak u novoj liniji
  background(0);
}

void draw() {
  
  stroke(pcolor);    // postavljanje point boje sa color vrijednostima
  point(xVal,yVal);
}

void serialEvent(Serial myPort) {
  
  String inString = myPort.readStringUntil('\n');    // dobijanje ASCII stringa
  
  if (inString !=null) {
    
    inString = trim(inString);    // uklanjanje bijelog prostora
    
    float[] colors = float(split(inString, ","));    // razdvajanje stringa na zarezu i konvertiranje rezultirajućih podstringova u niz cijelih brojeva
    
    if(colors.length >=3) {      // ako niz ima bar 3 elementa, znamo da smo dobili cijelu stvar. Zatim stavljamo brojeve u color varijable
      
      xVal = colors[0];          // mapiranje u rasponu od 0-255
      yVal = colors[1];
      pcolor = map(colors[2], 0, 400, 255,0);
     
      
      
    }
  }
}