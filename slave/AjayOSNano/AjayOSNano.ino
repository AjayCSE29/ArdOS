#include "worker.h"
#include "protocol.h"

void setup()
{
    Serial.begin(115200);

    workerBegin();

    Serial.println("Worker Ready");
}

void loop() {
  if(command == CMD_START_APP)
{
    if(parameter == APP_TETRIS)
    {
        Serial.println("Received START_APP");
    }

    command = 0;
}
}
