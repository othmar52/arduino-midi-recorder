

/**
   This "function" would normally crash any kernel that tries
   to run it by violating memory access. Instead, the Arduino's
   watchdog will auto-reboot, giving us a software "reset".
*/
void(* resetArduino) (void) = 0;

/**
  if we've not received any data for 2 minutes, and we were
  previously recording, we reset the arduino so that when
  we start playing again, we'll be doing so in a new file,
  rather than having multiple sessions with huge silence
  between them in the same file.
*/
void checkReset() {
  if (startTime == 0) return;
  if (!file) return;
  if (currentMillis - lastTime > RECORDING_TIMEOUT) {
    file.close();
    resetArduino();
  }
}
