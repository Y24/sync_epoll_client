#ifndef _DIRTY_TALK_GENERATOR_H
#define _DIRTY_TALK_GENERATOR_H 1
#include <random>
#include <string>
#include <vector>
class DirtyTalkGenerator {
 private:
  std::string pool[13] = {"请看清楚命令格式介绍噢",
                          "你会不会打命令？",
                          "连命令都打不明白？",
                          "求求让我少写点if吧",
                          "你应该回炉重造",
                          "抱歉，这条命令我识别不了",
                          "对方看不懂你的命令并释放了嘲讽技能",
                          "再乱输入直接宕机噢",
                          "命令就三种，您能记不住？",
                          "所以为什么相互折磨？",
                          "我累了，可以直接ctrl-D了",
                          "错误可以被人原谅，可惜我不是人",
                          "poweroff吧，这样对大家都好"};

 public:
  DirtyTalkGenerator() = default;
  // Generate a piece of elegant phrase.
  std::string generator() {
    std::random_device rd;
    std::default_random_engine rand(rd());
    return pool[rand() % 13];
  }
  static std::string welcome() {
    return "Welcome, supported command structure:\n\t1: start {nSession} "
           "\n\t2: test {nContent} "
           "\n\t3: stop\n\t4: exit\n";
  }
  ~DirtyTalkGenerator() = default;
};

#endif  // dirty_talk_generator.h