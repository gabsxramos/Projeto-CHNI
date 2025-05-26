# Projeto-CHNI
Projeto em parceria com a empresa CHNI, com objetivo de criar uma forma de sinalização sobre informações das peças dos carrinhos de montagem utilizados na empresa.

Durante o projeto foi, realizado a implementação de duas api necessárias para que exista a integração entre informações referentes a quantiedade de peças dos carrinhos de montagem, vindas dos projetos com o esp32, que seriam implantados nos carrinhos para essa coleta de dados. Após isso, os dados teriam de ser passados para um broker pela questão do manejo dos dados, e, em seguida, repassar os mesmos para a empresa, que utilizaria os dados e faria a reposição das peças nos carrinhos de montagem.

Para as APIs, foram feitam uma utilizando o BrokerMQTT HiveMQ e a outra o Mosquitto. Informações a respeito destes estão nos READ.ME, dentro de suas devidas pastas 
