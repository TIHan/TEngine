#include <engine_core.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, LoopTest) {
  auto event_processor = std::make_shared<EventProcessor>();
  auto event_aggregator = std::make_shared<EventAggregator>(event_processor);
  core::GameLoop g1(event_processor, event_aggregator, 20);

  std::thread loop1([&g1] {
    network::Server server(1337);
    server.Start();

    g1.Run([&server] {
      server.ProcessMessages();
      server.SendMessages();      
    });
  });

  std::chrono::milliseconds delay(500);
  std::this_thread::sleep_for(delay);

  auto event_processor2 = std::make_shared<EventProcessor>();
  auto event_aggregator2 = std::make_shared<EventAggregator>(event_processor2);
  core::GameLoop g2(event_processor2, event_aggregator2, 20);


  std::thread loop2([&g2, &event_aggregator2] {
    network::Client client(event_aggregator2);
    client.Connect("127.0.0.1", "1337");

    g2.Run([&client] {
      client.ProcessMessages();
      client.SendMessages();
    });
  });

  std::cout << "Press any key to end LoopTest." << std::endl;
  std::cin.get();
  g1.Stop();
  g2.Stop();
  loop1.join();
  loop2.join();
}