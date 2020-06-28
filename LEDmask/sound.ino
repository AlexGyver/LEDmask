int getSoundLength() {
  int thisMax = 0;
  int thisSignal = 0;
  for (int i = 0; i < 200; i++) {       // выборка 200 измерений
    thisSignal = analogRead(ADC_PIN);   // текущее измерение
    if (thisSignal > thisMax) thisMax = thisSignal; // ищем макс как больше большего
  }
  static float filMax = thisMax;
  filMax += (thisMax - filMax) * 0.95;   // фильтруем максимумы для плавности
  static float steady = thisMax;

  // фильтруем "тишину" с малым коэффициентом
  // + порог тишины в 30 единиц
  if (filMax - steady < 30) steady += (filMax - steady) * 0.005;
  if (steady > filMax) steady = filMax;

  if (filMax - steady > VOL_THR) return (constrain(map(filMax - steady - VOL_THR, 0, VOL_MAX, 0, 7), 0, 7));
  else return 0;
}

void analyzeAudio() {
  for (int i = 0 ; i < FHT_N ; i++) {
    int sample = analogRead(FREQ_PIN);
    fht_input[i] = sample; // put real data into bins
    delayMicroseconds(500);
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
}
