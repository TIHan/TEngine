#include <engine_core.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, LoopTest) {
  core::GameLoop g1(std::make_unique<EventSystem>(), 20);

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

  std::cout << "Press any key to end LoopTest." << std::endl;
  std::cin.get();
  g1.Stop();
  g2.Stop();
  loop1.join();
  loop2.join();
}