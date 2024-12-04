#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <Stepper.h>

// Configuración de la red WiFi
const char* ssid = "INFINITUM159C";
const char* password = "Pn5Lg5Zv3n";

// Configuración del servidor MQTT
const char* mqtt_server = "4ecbd73afda44bf09ed508ae41b86c40.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "leo_omtz";
const char* mqtt_password = "HiddenStar22";

// Canales MQTT para controlar los relés y el PWM
const char* CONTROL_RELES_TOPIC = "control-reles";
const char* CONTROL_PWM_TOPIC = "control-pwm";
const char* CONTROL_MOTOR_TOPIC = "control-motor";

// Pines para los relés
const int relePin1 = 26;
const int relePin2 = 27;
const int pwmPin = 25; // Pin para controlar el PWM (LED)

// Pines para el motor a pasos
const int stepsPerRevolution = 2048;  // Número de pasos por revolución
Stepper myStepper(stepsPerRevolution, 12, 14, 27, 26);  // Motor a pasos

// Certificado raíz
static const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Activar relé
void activarRele(int pin) {
  digitalWrite(pin, HIGH); // Activar el pin
  delay(1000);             // Mantener activado durante 1 segundo
  digitalWrite(pin, LOW);  // Apagar el pin
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Asegurarse de que el mensaje sea una cadena
  String mensaje = String((char*)payload);

  Serial.print("Mensaje recibido en el canal: ");
  Serial.println(topic);
  Serial.print("Contenido: ");
  Serial.println(mensaje);

  if (strcmp(topic, CONTROL_RELES_TOPIC) == 0) {
    if (mensaje == "E") {
      Serial.println("Activando relé 1...");
      activarRele(relePin1);
    } else if (mensaje == "A") {
      Serial.println("Activando relé 2...");
      activarRele(relePin2);
    } else {
      Serial.println("Comando no reconocido.");
    }
  }
  
  // Control de PWM
  if (strcmp(topic, CONTROL_PWM_TOPIC) == 0) {
    int pwmValue = mensaje.toInt();
    if (pwmValue >= 0 && pwmValue <= 255) {
      analogWrite(pwmPin, pwmValue);  // Controlar el brillo del LED
      Serial.print("PWM set to: ");
      Serial.println(pwmValue);
    }
  }

  // Control del motor a pasos
  if (strcmp(topic, CONTROL_MOTOR_TOPIC) == 0) {
    int steps = mensaje.toInt();
    if (steps != 0) {
      myStepper.step(steps);  // Mover el motor a pasos
      Serial.print("Motor moved: ");
      Serial.println(steps);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(CONTROL_RELES_TOPIC);
      client.subscribe(CONTROL_PWM_TOPIC);
      client.subscribe(CONTROL_MOTOR_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(relePin1, OUTPUT);
  pinMode(relePin2, OUTPUT);
  pinMode(pwmPin, OUTPUT);  // Configurar pin PWM como salida
  digitalWrite(relePin1, LOW); // Asegurarse de que el relé esté apagado al iniciar
  digitalWrite(relePin2, LOW); // Asegurarse de que el relé esté apagado al iniciar

  Serial.begin(115200);
  setup_wifi();
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
