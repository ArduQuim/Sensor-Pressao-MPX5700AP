#define pinoSensor A0 // Define o pino do Arduino que recebera os dados do sensor de gás

int medida; // Variável que armazenará o valor correspondente à voltagem vinda do sensor
float pressao; // Variável que armazenará o valor de pressão obtida em quilopascal (kPa)
float pressaoAtm, pressaoBar, pressaoKgfcm2; // Variáveis que armazenarão o valor da pressão obtida, mas em outras unidades de medidas

// Variáveis que são utilizadas para converter o valor de pressão de kPa para atm, bar ou kgf/cm²
const float kPa_atm = 0.0098692327;
const float kPa_bar = 0.1;
const float kPa_kgfcm2 = 0.0101971621;

// Comandos executados ao inicializar o Arduino 
void setup() { 

  Serial.begin(9600); // Inicia a porta serial com velocidade de 9600 

  pinMode(pinoSensor, INPUT); // Declara o pino do sensor para entrada de dados
} 

// Comandos que serao executados repetidamente (loop)
void loop() {

  medida = analogRead(pinoSensor); // Lê a voltagem enviada pelo sensor e a armazena como um valor entre 0 e 1023

  pressao = ((float(medida) / 1023) - 0.04) / 0.0012858; // Interpreta o valor adquirido, obtendo a pressão em kPa e armazenando-a na variável "pressao"
  // OBS: Essa converção é realizada com o auxílio da equação descrita na datasheet do aparelho: Vout = Vs * (0.0012858 * P + 0.04) ± Erro

  // Os proximos comandos imprimem a pressão na porta serial
  Serial.print("Pressão: "); // Imprime o tempo "Pressão: "
  Serial.print(pressao); // Imprime o valor da pressão obtida em kPa
  Serial.print(" kPa"); // Imprime a unidade de medida (kPa)

  // O resultado fica como:
  /*
  Pressão: 105 kPa
  */

  pressaoAtm = pressao * kPa_atm; // Converte o valor da pressão de quilopascal (kPa) para atm (atmosfera padrão)
  Serial.print(" | "); // Imprime um símbolo para separar os valores
  Serial.print(pressaoAtm); // Imprime o valor da pressão obtida em atm
  Serial.print(" atm"); // Imprime a unidade de medida (atm)
  
  pressaoBar = pressao * kPa_bar; // Converte o valor da pressão de quilopascal (kPa) para bar
  Serial.print(" | "); // Imprime um símbolo para separar os valores
  Serial.print(pressaoBar); // Imprime o valor da pressão obtida em bar
  Serial.print(" bar"); // Imprime a unidade de medida (bar)
  
  pressaoKgfcm2 = pressao * kPa_kgfcm2; // Converte o valor da pressão de quilopascal (kPa) para kgf/cm² (quilograma força por centímetro quadrado)
  Serial.print(" | "); // Imprime um símbolo para separar os valores
  Serial.print(pressaoKgfcm2); // Imprime o valor da pressão obtida em kgf/cm²
  Serial.print(" kgf/cm²"); // Imprime a unidade de medida (kgf/cm²)
  
  Serial.println(); // Imprime uma quebra de linha 
  
  delay(3000); // Estabelece um intervalo em milisegundos entre as medicoes (3000 ms = 3 s)

}
