void setup()
{
  Serial.begin(115200);
  Serial.print("R\n"); // Reset graphing window
  Serial.print("R\n"); //  "It is recommended to start your graph with a couple of these commands to get the serial system into sync."
  Serial.print("R\n");
  Serial.print("S1280:800\n"); // Screen size x:y
  Serial.print("M10:10:60:20\n"); // Margins top:right:bottom:left
  Serial.print("B255:255:255\n"); // Background Color red:green:blue
  Serial.print("F0:0:0\n"); // Axis color red:green:blue
  Serial.print("Y0:5:1\n"); // Y axis scale min:max:step
  Serial.print("AanalogRead(0) / 1023.0 * 5.0:255:0:0\n"); // Data set and color title:r:g:b
  Serial.print("A(sin(f) + 1) * 2.5 ; f = f + ((cos(r)+1)/5.0) ; r = r + 0.005:0:255:0\n");
  Serial.print("A(sin(j)/2.0) + 2.5 ; j = j + 0.01:0:0:255\n");
}

void loop()
{
  static float r = 0.0;
  static float f = 0.001;
  static float j = 0.001;
  static unsigned long x = 0;
  float v1 = analogRead(0);
  float v2 = (sin(f) + 1) * 2.5;
  float v3 = (sin(j)/2.0) + 2.5;// * 2.5;
  v1 = v1 / 1023.0 * 5.0;
  f = f + ((cos(r)+1)/5.0);
  j = j + 0.01;
  r = r + 0.005;
 
  Serial.print("V"); // Add data point values a:b:c:...:n
  Serial.print(v1);
  Serial.print(":");
  Serial.print(v2);
  Serial.print(":");
  Serial.println(v3); // /r/n is interpreted the same as /n
  delay(25);
}