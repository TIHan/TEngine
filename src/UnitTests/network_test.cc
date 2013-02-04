#include <engine_core.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, NetworkLoopTest) {
  auto system1 = std::make_unique<EventSystem>();
  auto aggregator1 = system1->GetAggregator();
  core::GameLoop g1(std::move(system1), 20);

  std::thread loop1([&g1, &aggregator1] {
    network::Server server(aggregator1, 1337);
    server.Start();

    g1.Run([&server] {
      server.ProcessMessages();
      server.SendMessages();      
    });
  });
    
  std::chrono::milliseconds delay(500);
  std::this_thread::sleep_for(delay);

  auto system2 = std::make_unique<EventSystem>();
  auto aggregator2 = system2->GetAggregator();
  core::GameLoop g2(std::move(system2), 20);

  std::thread loop2([&g2, &aggregator2] {
    network::Client client(aggregator2);
    client.Connect("127.0.0.1", "1337");

    g2.Run([&client] {
      client.ProcessMessages();
      client.SendMessages();
    });
  });

  std::cin.get();
  g1.Stop();
  g2.Stop();
  loop1.join();
  loop2.join();
}