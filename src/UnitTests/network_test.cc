#include <engine_network.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

#define TICK_RATE 25;

TEST_F(NetworkTest, LoopTest) {
  std::atomic<bool> shutdown;
  shutdown = false;

  std::thread loop1([&shutdown] {
    network::Server server(1337);
    server.Start();

    std::chrono::high_resolution_clock clock;
    int rate = ((1.0 / 25) * 1000);
    std::chrono::milliseconds tick_rate(rate);

    while(!shutdown) {
      auto time = clock.now();

      server.ProcessMessages();
      server.SendMessages();

      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - time);
      std::this_thread::sleep_for(tick_rate - duration);
    }
  });

  std::chrono::milliseconds delay(500);
  std::this_thread::sleep_for(delay);

  std::thread loop2([&shutdown] {
    network::Client client;
    client.Connect("127.0.0.1", "1337");

    std::chrono::high_resolution_clock clock;
    int rate = ((1.0 / 25) * 1000);
    std::chrono::milliseconds tick_rate(rate);

    uint64_t ticks = 1;
    while(!shutdown) {
      auto time = clock.now();
      client.ProcessMessages();
      if (ticks % 25 == 0) {
        client.SendHeartbeat();
      }
      client.SendMessages();

      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - time);
      std::this_thread::sleep_for(tick_rate - duration);
      //std::cout << "Process Time: " << duration.count() << std::endl;
      ticks++;
    }
  });

  //std::chrono::milliseconds time(500);
  //network::Server server(1337);
  //network::Client client;
  //network::Client client2;

  //server.Start();
  //client.Connect("127.0.0.1", "1337");
  //client2.Connect("127.0.0.1", "1337");
  //std::this_thread::sleep_for(time);
  //server.SendMessages();
  //std::this_thread::sleep_for(time);
  //client.ProcessMessages();
  //client2.ProcessMessages();

  //// heartbeat test
  //client.SendHeartbeat();
  //client.SendMessages();
  //std::this_thread::sleep_for(time);
  //server.ProcessMessages();
  //server.SendMessages();
  //std::this_thread::sleep_for(time);
  //client.ProcessMessages();
  //client2.ProcessMessages();
  std::cout << "Press any key to end LoopTest." << std::endl;
  std::cin.get();
  shutdown = true;
  loop1.join();
  loop2.join();
}