#include <iostream>

#include "api/Api.h"
#include "api/Subscription.h"
#include "api/Connection.h"

int main(int argc, char** argv) {
  dxfeed::perf::setProcessPriorityClass();
  // load cmd args
  const int defaultArgSize = 4;
  if (argc < defaultArgSize) {
    std::cerr << "Error: expected 3 args: <JAVA_HOME, address:port, symbol, ... VMOptions >" << std::endl;
    return -1;
  }
  const auto javaHomePath = argv[1];
  const int vmOptionsSize = 1;
  const char* jvmArgs[vmOptionsSize] = { "-Xmx12G" };
  const auto address = argv[2];
  const auto symbol = argv[3];

  // init context, connection, subscription
  std::cout << "Connection to address:" << address << std::endl;
  dxfeed_init(javaHomePath, jvmArgs, vmOptionsSize);
  void* dxFeed = dxfeed_get_instance();
  auto connection = dxfeed_create_connection(dxFeed, address);
  auto subscription = dxfeed_create_subscription(connection, 0);

  // add listener with user code
  dxfeed_add_listener(subscription, [](const void *events, int count) {
    auto timeAndSaleList = reinterpret_cast<const TimeAndSale*>(events);
    for (int i = 0; i < count; ++i) {
      auto quote = std::make_shared<TimeAndSale>(timeAndSaleList[i]);
      std::cout << dxfeed::TimeAndSaleFormatter::toString(quote.get()) << std::endl;
    }
  });

  // add symbol to subscription
  dxfeed_add_symbol(subscription, symbol);

    // sleep then clean up
  std::chrono::hours hours(24); // time to sleep 24 hours
  std::this_thread::sleep_for(hours);
  delete (dxfeed::Subscription*)subscription;
  delete (dxfeed::Connection*) connection;
}