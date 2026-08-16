import rclpy

from rclpy.node import Node

from std_msgs.msg import Bool

from std_msgs.msg import Int32

class SensorLed(Node):

    def __init__(self):
        super().__init__("nó")
        # Publisher
     
        # Subscribe
       
        # Variáveis
      
        # Timer
      
    def callback(self, msg):
        
    def timer_callback(self):
   
def main(args=None):

    rclpy.init(args=args) #inicializa o ROS2

    node = SensorLed() #atribui a classe criada ao nó

    rclpy.spin(node) #cria como um looping de execução esperando eventos

    node.destroy_node() #destrói o nó quando a execução para

    rclpy.shutdown() #encerra o ROS2


if __name__ == "__main__":
    main()


