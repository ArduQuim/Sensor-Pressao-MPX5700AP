#include <LiquidCrystal_I2C.h> // Gere o display LCD
#include <SD.h> // Gere o cartão de memoria e os dados nele armazenados
#define pinoSensor A0 // Define o pino do Arduino que recebera os dados do sensor de gás

int medida; // Variável que armazenará o valor correspondente à voltagem vinda do sensor
float pressao; // Variável que armazenará o valor de pressão obtida em quilopascal (kPa)
float pressaoAtm, pressaoBar, pressaoKgfcm2; // Variáveis que armazenarão o valor da pressão obtida, mas em outras unidades de medidas

// Variáveis que são utilizadas para converter o valor de pressão de kPa para atm, bar ou kgf/cm²
const float kPa_atm = 0.0098692327;
const float kPa_bar = 0.1;
const float kPa_kgfcm2 = 0.0101971621;

const int LCD_linhas = 2; // Número de linhas do seu display LCD
const int LCD_colunas = 16; // Número de colunas do seu display LCD
const int numeroPiscadas = 3; // Número de vezes que o dado pisca
// As próximas 3 variáveis descrevem os intervalos de tempo empregados no display LCD e podem ser editados conforme o desejo do usuário
// OBS: Como todo o processo de requisitar, receber e interpretar o dado de temperatura leva, aproximadamente, 0.5 seg, para manter um intervalo total de cerca de 5 seg, a soma das variáveis abaixo foi ajustada para cerca de 4.5 seg
const float LCD_tempoAceso = 2.5; // Tempo, em segundos, em que o dado é exibido
const float LCD_tempoPiscando = 1.8; // Tempo, em segundos, em que o dado permanece piscando
const float LCD_tempoApagado = 0.2; // Intervalo, em segundos, entre a exibição dos dados

LiquidCrystal_I2C lcd(0x27, LCD_colunas, LCD_linhas); // Declara o display como um objeto chamado "lcd"

// Para o cartão SD, 3 pinos já são determinados
// SCK = 13;
// MISO = 12;
// MOSI = 11;
const int CS = 10; // Determina o ultimo pino

unsigned long tempoColeta;

// Comandos executados ao inicializar o Arduino
void setup() {

  Serial.begin(9600); // Inicia a porta serial com velocidade de 9600

  pinMode(pinoSensor, INPUT); // Declara o pino do sensor para entrada de dados

  lcd.init(); // Inicia o display lcd(x, y), de x colunas e y linhas; Nesse caso, lcd(16,2);
  lcd.backlight(); // Liga a luz de fundo do display
  lcd.setCursor(0, 0); // Posiciona o cursor na primeira coluna (0) e na primeira linha (0)
  lcd.print("Pressao: "); // Imprime, na primeira linha, o texto “Pressao:”
  Serial.println("Display LCD ligado"); // Informa, na porta serial, que o display esta ligado

  Serial.println("Iniciando cartão SD...");
  pinMode(CS, OUTPUT); // Configura o pino CS como saída

  if (SD.begin(CS)) { // Caso a inicialização do SD no pino CS ocorra com sucesso
    Serial.println("Cartão SD inicializado corretamente"); // Imprime uma mensagem de sucesso;
  } else { // Caso a inicialização do SD no pino CS falhe
    Serial.println("Falha na inicialização do cartão SD"); // Imprime uma mensagem de erro
    while (1); // Inicia um loop sem fim pois o cartão não foi devidamente iniciado
  }
}

// Comandos que serao executados repetidamente (loop)
void loop() {

  medida = analogRead(pinoSensor); // Lê a voltagem enviada pelo sensor e a armazena como um valor entre 0 e 1023

  pressao = ((float(medida) / 1023) - 0.04) / 0.0012858; // Interpreta o valor adquirido, obtendo a pressão em kPa e armazenando-a na variável "pressao"
  // OBS: Essa converção é realizada com o auxílio da equação descrita na datasheet do aparelho: Vout = Vs * (0.0012858 * P + 0.04) ± Erro

  // Os proximos comandos imprimem a pressão na porta serial
  Serial.print("Pressão: "); // Imprime o termo "Pressão: "
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

  lcd.backlight(); // Liga a luz de fundo do display

  // Os proximos comandos imprimem a temperatura no display LCD
  lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna (0) e na segunda linha (1)
  lcd.print(pressao); // Imprime o valor da pressão coletada
  lcd.print(" ");
  lcd.print(" atm"); // Imprime a unidade de medida (atm)

  // O resultado fica como:
  /*
    Pressão:
    1 atm
  */

  delay(LCD_tempoAceso * 1000); // Estabelece um intervalo em milisegundos entre as medicoes

  // Apaga e reescreve o valor no display, fazendo o valor piscar
  for (int i = 0; i < numeroPiscadas; i++) {
    lcd.setCursor(0, 1);
    for (int j = 0; j < LCD_colunas; j++) {
      lcd.print(" ");
    }

    delay((LCD_tempoPiscando * 1000) / (numeroPiscadas * 2)); // Gere uma pausa entre os processos de apagar e reescreer os dados

    lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna (0) e na segunda linha (1)
    lcd.print(pressao); // Imprime o valor da pressão coletada
    lcd.print(" ");
    lcd.print(" atm"); // Imprime a unidade atm

    delay((LCD_tempoPiscando * 1000) / (numeroPiscadas * 2)); // Gere uma pausa entre os processos de apagar e reescreer os dados
  }

  lcd.noBacklight(); // Desliga a luz de fundo do display

  delay(LCD_tempoApagado * 1000);
  
    // Abrir um arquivo no cartão SD e escrever nele
    File dataFile = SD.open("data.csv", FILE_WRITE); // Abre o arquivo data.csv e posiciona o cursor no seu fim
    
    if(dataFile){ // Caso o arquivo seja aberto com sucess  
      // Imprimir os dados no arquivo aberto
      dataFile.print(tempoColeta); // Imprime o tempo em milissegundos em que essa coleta foi realizada
      dataFile.print(","); // Imprime a vírgula para separar os dados
      dataFile.print(pressao); // Imprime a pressão coletada
      dataFile.println(); // Coloca uma quebra de linha
    
      // Fechar o arquivo e salvar as modificações
      dataFile.close();
    
    }


  delay(3000); // Estabelece um intervalo em milisegundos entre as medicoes (3000 ms = 3 s)

}
