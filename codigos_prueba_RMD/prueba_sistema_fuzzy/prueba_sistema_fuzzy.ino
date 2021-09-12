#include <Fuzzy.h>

// Fuzzy
Fuzzy *fuzzy = new Fuzzy();

// FuzzyInput
FuzzySet *pequenno = new FuzzySet(0, 0, 3, 6);
FuzzySet *mediano = new FuzzySet(4, 6, 8, 10);
FuzzySet *grande = new FuzzySet(8, 10, 12, 12);

// FuzzyInput
FuzzySet *recta = new FuzzySet(0, 0, 5, 10);
FuzzySet *curva_suave = new FuzzySet(5, 10, 15, 20);
FuzzySet *curva_cerrada = new FuzzySet(15, 20, 25, 25);


// FuzzyOutput
FuzzySet *simple = new FuzzySet(0, 0, 0.1, 0.2);
FuzzySet *doble_ponderado = new FuzzySet(0.2, 0.5, 0.5, 0.8);
FuzzySet *doble = new FuzzySet(0.8, 0.9, 1, 1);


void setup()
{
  // Set the Serial output
  Serial.begin(9600);
  // Set a random seed
  randomSeed(analogRead(0));

  // Every setup must occur in the function setup()

  // FuzzyInput
  FuzzyInput *error_abs = new FuzzyInput(1);

  error_abs->addFuzzySet(pequenno);
  error_abs->addFuzzySet(mediano);
  error_abs->addFuzzySet(grande);
  fuzzy->addFuzzyInput(error_abs);

  // FuzzyInput
  FuzzyInput *curvatura_abs = new FuzzyInput(2);

  curvatura_abs->addFuzzySet(recta);
  curvatura_abs->addFuzzySet(curva_suave);
  curvatura_abs->addFuzzySet(curva_cerrada);
  fuzzy->addFuzzyInput(curvatura_abs);


  // FuzzyOutput
  FuzzyOutput *controlador = new FuzzyOutput(1);

  controlador->addFuzzySet(doble);
  controlador->addFuzzySet(doble_ponderado);
  controlador->addFuzzySet(simple);
  fuzzy->addFuzzyOutput(controlador);

  // construyendo regla 1
  FuzzyRuleAntecedent *si_curva_cerrada_AND_error_pequenno = new FuzzyRuleAntecedent();
  si_curva_cerrada_AND_error_pequenno->joinWithAND(curva_cerrada, pequenno);

  FuzzyRuleConsequent *entonces_controlador_doblePonderado = new FuzzyRuleConsequent();
  entonces_controlador_doblePonderado->addOutput(doble_ponderado);

  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, si_curva_cerrada_AND_error_pequenno, entonces_controlador_doblePonderado);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // construyendo regla 2
  FuzzyRuleAntecedent *error_mediano_OR_grande = new FuzzyRuleAntecedent();
  error_mediano_OR_grande->joinWithOR(mediano, grande);
  FuzzyRuleAntecedent *curvaCerrada = new FuzzyRuleAntecedent();
  curvaCerrada->joinSingle(curva_cerrada);
  FuzzyRuleAntecedent *si_curva_cerrada_AND_error_mediano_OR_grande = new FuzzyRuleAntecedent();
  si_curva_cerrada_AND_error_mediano_OR_grande->joinWithAND(curvaCerrada, error_mediano_OR_grande);

  FuzzyRuleConsequent *entonces_controlador_simple = new FuzzyRuleConsequent();
  entonces_controlador_simple->addOutput(simple);

  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, si_curva_cerrada_AND_error_mediano_OR_grande, entonces_controlador_simple);
  fuzzy->addFuzzyRule(fuzzyRule2);

  // construyendo regla 3
  FuzzyRuleAntecedent *si_curva_suave_AND_error_pequenno = new FuzzyRuleAntecedent();
  si_curva_suave_AND_error_pequenno->joinWithAND(curva_suave, pequenno);

  FuzzyRuleConsequent *entonces_controlador_doble = new FuzzyRuleConsequent();
  entonces_controlador_doble->addOutput(doble);

  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, si_curva_suave_AND_error_pequenno, entonces_controlador_doble);
  fuzzy->addFuzzyRule(fuzzyRule3);

  // construyendo regla 4
  FuzzyRuleAntecedent *si_curva_suave_AND_error_mediano = new FuzzyRuleAntecedent();
 si_curva_suave_AND_error_mediano->joinWithAND(curva_suave, mediano);

  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, si_curva_suave_AND_error_mediano, entonces_controlador_doblePonderado);
  fuzzy->addFuzzyRule(fuzzyRule4);

  // construyendo regla 5
  FuzzyRuleAntecedent *si_curva_suave_AND_error_grande = new FuzzyRuleAntecedent();
  si_curva_suave_AND_error_grande->joinWithAND(curva_suave, grande);


  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, si_curva_suave_AND_error_grande, entonces_controlador_simple);
  fuzzy->addFuzzyRule(fuzzyRule5);

  // construyendo regla 6
  FuzzyRuleAntecedent *si_recta = new FuzzyRuleAntecedent();
  si_recta->joinSingle(recta);


  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, si_recta, entonces_controlador_doble);
  fuzzy->addFuzzyRule(fuzzyRule6);
}

void loop()
{
  // get random entrances
  float input1 = random(0, 29.9);
  float input2 = random(0, 59.9);

  Serial.println("\n\n\nEntrance: ");
  Serial.print("\t\t\tError(abs): ");
  Serial.print(input1);
  Serial.print(", Curvatura: ");
  Serial.println(input2);
  uint32_t t_v=micros();                     // para imprimir el tiempo de computo
  fuzzy->setInput(1, input1);
  fuzzy->setInput(2, input2);

  fuzzy->fuzzify();
/*
  Serial.println("Input: ");
  Serial.print("\tError: pequenno-> ");
  Serial.print(pequenno->getPertinence());
  Serial.print(", Mediano-> ");
  Serial.print(mediano->getPertinence());
  Serial.print(", Grande-> ");
  Serial.println(grande->getPertinence());

  Serial.print("\tCurvatura: Recta-> ");
  Serial.print(recta->getPertinence());
  Serial.print(",  Curva_Suave-> ");
  Serial.print(curva_suave->getPertinence());
  Serial.print(",  Curva_Cerrada-> ");
  Serial.println(curva_cerrada->getPertinence());
*/
  float output1 = fuzzy->defuzzify(1);
  uint32_t time_now = micros() - t_v;      //
  Serial.print("Retardo: ");               //   para imprimir el tiempo de computo
  Serial.println(time_now);                //
  Serial.println("Output: ");
  Serial.print("\tControlador: Doble-> ");
  Serial.print(doble->getPertinence());
  Serial.print(", Doble_Ponderado-> ");
  Serial.print(doble_ponderado->getPertinence());
  Serial.print(", Simple-> ");
  Serial.println(simple->getPertinence());


  Serial.println("Result: ");
  Serial.print("\t\t\tControlador: ");
  Serial.println(output1);

  // wait 12 seconds
  delay(12000);
}
