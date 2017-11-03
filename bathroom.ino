#include <DHT.h>
#include <DHT_U.h>

#define PIN_DHT (4)
#define PIN_VENTA (3)

//#define DHT_DEBUG

#define HUMIDITY_THRESHOLD_ON  (76)
#define HUMIDITY_THRESHOLD_OFF (72)

DHT_Unified dht(PIN_DHT, DHT22);

static bool venta = false;
static int dht_ok_cnt = 0;

void setup()
{
  digitalWrite(PIN_VENTA, HIGH);
  pinMode(PIN_VENTA, OUTPUT);

  dht.begin();

#ifdef DHT_DEBUG
  Serial.begin(115200);
#endif
}

void loop()
{
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  delay (1000);
  dht.humidity().getEvent(&event);

  if (!isnan(event.relative_humidity)) {
    dht_ok_cnt = 10;

    if (!venta) {
      if (event.relative_humidity > HUMIDITY_THRESHOLD_ON) {
        venta = true;
      }
    } else {
      if (event.relative_humidity < HUMIDITY_THRESHOLD_OFF) {
        venta = false;
      }
    }
  } else {
    dht_ok_cnt = max(dht_ok_cnt-1, 0);
    if (!dht_ok_cnt) {
      // no reading for 10 cycles, turn off venta regardless of its state
      venta = false;
    }
  }

  digitalWrite(PIN_VENTA, venta ? LOW : HIGH);

#ifdef DHT_DEBUG
  // need to update
  Serial.print(dht.humidity, 1);
  Serial.print("\t");
  Serial.print(dht.temperature, 1);
  Serial.print("\t");
  Serial.print(dht_ok_cnt);
  Serial.println();
#endif

  delay(4000);
}

