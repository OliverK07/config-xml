#include "Conn.h"
#include "Logger.h"
#include "Path.h"
#include "SaxParser.h"
#include "Configuration.h"

int main()
{
   std::cout << "\nDemo XML configuration file\n";

   conn::Logger *logger (conn::Logger::root()->getChild ("Container"));
   conn::SaxParser parser (logger->getChild ("xmlparser"));
   conn::Path configfile ("modbus1.xml");

   parser.loadGrammar ("ModbusConnector.xsd");
   conn::HConfiguration config (conn::Configuration::load (parser, configfile));

   bool ret = false;
   if (config->hasChild ("connection"))
   {
      const conn::Configuration *connection = config->getChild ("connection");
      if (connection->hasChild ("tcp"))
      {
         const conn::Configuration *tcp = connection->getChild ("tcp");
         std::string ip (tcp->getChild("ip")->getValueAsString ());
         uint32_t port (tcp->getChild("port")->getValueAsInteger ());

         std::cout << "The connection part of modbus1.xml show as: " << std::endl;
         std::cout << "connection: TCP " << std::endl;
         std::cout << "IP: " << ip << std::endl;
         std::cout << "Port: " << port << std::endl;
         ret = true;
      }
   }

   if (ret == false)
   {
      std::cout << "FAIL: Check the code" << std::endl;
   }

   return ret;
}