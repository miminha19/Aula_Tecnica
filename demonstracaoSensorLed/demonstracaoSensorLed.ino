#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/bool.h>

#define LED_PIN 25    
#define SENSOR 13

#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

 char ssid[] = "wIFFadministrativa";
 char password[] = "pOLO@2016";
 char agent_ip[] = "10.80.4.168";

rcl_allocator_t allocator;
rcl_node_t node;
rcl_subscription_t subscriber;
rcl_publisher_t publisher;
rclc_support_t support;
rclc_executor_t executor;


// Mensagem enviada
std_msgs__msg__Bool sensor_msg;

//Mensagem recebida
std_msgs__msg__Int32 led_msg;


// ----------------------
// Callback
// ----------------------
void subscription_callback(const void* msgin){
    const std_msgs__msg__Int32 * msg =
 (const std_msgs__msg__Int32 *) msgin;

   int pwm = msg -> data;
   Serial.print("PWM: ");
   Serial.println(pwm);
   
    ledcWrite(LED_PIN, pwm);
}

// ----------------------
// Setup
// ----------------------
void setup(){


    Serial.begin(115200);
    // Configuração do LED
    pinMode(LED_PIN, OUTPUT);
    pinMode(SENSOR, INPUT);

    ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);

 //Comunicação wifi com o Agent
    set_microros_wifi_transports(
        ssid,
        password,
        agent_ip,
        8888
    );

   delay(2000);

  //Inicialização do micro-ROS
    allocator = rcl_get_default_allocator();

    rclc_support_init(
        &support,
        0,
        NULL,
        &allocator);

    // Criação do nó
    rclc_node_init_default(
        &node,
        "esp32_node",
        "",
        &support);

    // Criação do subscriber
    rclc_subscription_init_default(
        &subscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "/led");

    rclc_publisher_init_default(
        &publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "/sensor");

    // Criação do executor
    rclc_executor_init(
        &executor,
        &support.context,
        1,
        &allocator);

    // Adiciona o subscriber ao executor
    rclc_executor_add_subscription(
        &executor,
        &subscriber,
        &led_msg,
        &subscription_callback,
        ON_NEW_DATA);
        
}
// ----------------------
// Loop
// ----------------------
void loop(){

int sensor = digitalRead(SENSOR);

if(sensor == 1){
       sensor_msg.data = false;
    }

    else if(sensor == 0){
       sensor_msg.data = true;
    }

    rclc_executor_spin_some(
        &executor,
        RCL_MS_TO_NS(10));

    rcl_publish(
        &publisher,
        &sensor_msg,
        NULL);

    delay(10);

}

