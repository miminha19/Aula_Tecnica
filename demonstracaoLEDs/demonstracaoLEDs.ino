#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/bool.h>

char ssid[] = "wIFFadministrativa";
char password[] = "pOLO@2016";
char agent_ip[] = "10.80.4.168";

rcl_allocator_t allocator;
rclc_support_t support;
rclc_executor_t executor;
rcl_node_t node;
//rcl_publisher_t publisher;
rcl_subscription_t subscriber_led1;
rcl_subscription_t subscriber_led2;
rcl_subscription_t subscriber_led3;
rcl_subscription_t subscriber_led4;

//Mensagem recebida
std_msgs__msg__Bool msg_led1;
std_msgs__msg__Bool msg_led2;
std_msgs__msg__Bool msg_led3;
std_msgs__msg__Bool msg_led4;

//LEDS
#define LED1 25
#define LED2 33
#define LED3 32
#define LED4 26


//========================
// Callback
//========================


void callback_led1(const void * msgin)
{
    const std_msgs__msg__Bool * msg =
        (const std_msgs__msg__Bool *) msgin;

    digitalWrite(LED1, msg->data);
}
void callback_led2(const void * msgin)
{
    const std_msgs__msg__Bool * msg =
        (const std_msgs__msg__Bool *) msgin;

    digitalWrite(LED2, msg->data);
}
void callback_led3(const void * msgin)
{
    const std_msgs__msg__Bool * msg =
        (const std_msgs__msg__Bool *) msgin;

    digitalWrite(LED3, msg->data);
}
void callback_led4(const void * msgin)
{
    const std_msgs__msg__Bool * msg =
        (const std_msgs__msg__Bool *) msgin;

    digitalWrite(LED4, msg->data);
}




//========================
// Setup
//========================


void setup()
{
    Serial.begin(115200);
   
    set_microros_wifi_transports(
        ssid,
        password,
        agent_ip,
        8888
    );
    delay(2000);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);

    allocator = rcl_get_default_allocator();

    rclc_support_init(
        &support,
        0,
        NULL,
        &allocator
    );

    rclc_node_init_default(
        &node,
        "esp32_node",
        "",
        &support
    );

    rclc_subscription_init_default(
        &subscriber_led1,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "/led1"
    );

    rclc_subscription_init_default(
        &subscriber_led2,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "/led2"
    );

    rclc_subscription_init_default(
        &subscriber_led3,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "/led3"
    );

    rclc_subscription_init_default(
        &subscriber_led4,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "/led4"
    );

    rclc_executor_init(
        &executor,
        &support.context,
        4,
        &allocator
    );

    rclc_executor_add_subscription(
    &executor,
    &subscriber_led1,
    &msg_led1,
    &callback_led1,
    ON_NEW_DATA
);

rclc_executor_add_subscription(
    &executor,
    &subscriber_led2,
    &msg_led2,
    &callback_led2,
    ON_NEW_DATA
);

rclc_executor_add_subscription(
    &executor,
    &subscriber_led3,
    &msg_led3,
    &callback_led3,
    ON_NEW_DATA
);

rclc_executor_add_subscription(
    &executor,
    &subscriber_led4,
    &msg_led4,
    &callback_led4,
    ON_NEW_DATA
);
}
//========================
// Loop
//========================


void loop()
{
    rclc_executor_spin_some(
        &executor,
        RCL_MS_TO_NS(1)
    );

}

