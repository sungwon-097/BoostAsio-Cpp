# ChatServer-C++

### Environment
- Ubuntu 22.04
- g++ (Ubuntu 11.3.0-1ubuntu1~22.04.1) 11.3.0
- cmake version 3.22.1
- boost_1_82_0

### Description
React Client와 통신하며 채팅서버 구현
1. 접속한 모든 Client에게 Boradcast하는 채팅 서버
2. 룸단위로 나누어 관리하는 채팅 서버
3. 멀티스레딩을 적용해 대규모 트래픽을 고려한 채팅서버