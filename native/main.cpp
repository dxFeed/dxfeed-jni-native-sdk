#include <iostream>

#include "api/Api.h"
#include "api/Subscription.h"
#include "api/Connection.h"
#include "api/utils/Diagnostic.h"

void simpleListener(void* dxFeedSubscription) {
  dxfeed_add_listener(dxFeedSubscription, [](const void *events, int count) {
    auto timeAndSaleList = reinterpret_cast<const TimeAndSale*>(events);
    for (int i = 0; i < count; ++i) {
      auto quote = std::make_shared<TimeAndSale>(timeAndSaleList[i]);
      std::cout << dxfeed::TimeAndSaleFormatter::toString(quote.get()) << std::endl;
    }
  });
}

// todo: replace first arg in shared configuration (JAVA_HOME path):
//  D:\Tools\labsjdk-ce-17.0.6-jvmci-22.3-b13 demo.dxfeed.com:7300 ETH/USD:GDAX
int main(int argc, char** argv) {
  dxfeed::perf::setProcessPriorityClass();
  const int defaultArgSize = 4;
  if (argc < defaultArgSize) {
    std::cerr << "Error: expected 3 args: <JAVA_HOME, address:port, symbol, ... VMOptions >" << std::endl;
    return -1;
  }
  const auto java_home_path = argv[1];
  dxfeed_init(java_home_path, argv + defaultArgSize, argc - defaultArgSize);
  const auto address = argv[2];
  std::cout << "Connection to address:" << address << std::endl;
  const auto symbol = argv[3];
  void* dxFeed = dxfeed_get_instance();
  auto connection = dxfeed_create_connection(dxFeed, address);
  auto subscription = dxfeed_create_subscription(connection, 0);
  std::cout << "dx subscription wrapper: " << static_cast<dxfeed::Subscription*>(subscription) << std::endl;
  std::cout << "dx subscription jobject: " << static_cast<dxfeed::Subscription*>(subscription)->getSub() << std::endl;

//  simpleListener(subscription);  // Case 1 -> Simple Listener
//  auto listener = std::make_unique<dxfeed::perf::Diagnostic>(2); // Case 2 -> PerfTest Listener
  auto listener = std::make_unique<dxfeed::perf::Receiver>();      // Case 3 -> Receiver Listener
  dxfeed_add_diagnostic_listener(subscription, reinterpret_cast<int64_t>(listener.get()));

  dxfeed_add_symbol(subscription, symbol);
  std::cout << "Subscribe to symbol:" << symbol << std::endl;
  std::chrono::hours hours(24); // time to sleep 24 hours
  std::this_thread::sleep_for(hours);
  delete (dxfeed::Subscription*)subscription;
  delete (dxfeed::Connection*) connection;
}