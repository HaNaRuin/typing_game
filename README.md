# Tyging Game 산성비
 기존의 한컴 타자 연습에 있는 타자 게임 중 산성비 게임을 C언어로 구현하여 콘솔 창에서 할 수 있는 쉬운 타자 연습 게임이다. 

 산성비 게임은 하늘에서 내려오는 단어들을 빨리 타이핑하여 땅에 닿지 않게 하는 게임으로써 사용자의 타이핑 속도를 증가시키고 타이핑의 정확성을 기르는 게임이며 스테이지는 총 4개로 구성되어있다. 다음 스테이지로 이동할 때마다 단어가 떨어지는 속도가 증가하며 일정 개수의 단어 입력이 틀리면 프로그램이 종료된다.

## 프로젝트 개발 기간 및 환경
* **기간 :** 2014-11-12 ~ 2014-12-01
* **OS :** Window 8.1
* **Language :** C
* **Tools :** Visual Studio 2013 

## 개발 내용
산성비 게임의 기본적인 동작 이외에 구현한 내용 :
* 총 4단계의 STAGE로 구현하였고 STAGE를 통과할 때마다 단어가 내려오는 속도가 빨라짐
* 게임 시작 시 바로 시작이 아닌 사용자가 enter key를 누르면 시작
* 떨어지는 단어들을 파일 입출력을 통해 텍스트 파일에서 가져옴

## Source
* 게임 틀을 그리기 위한 커서를 제어하는 함수와 단어의 속도를 제어하기위한 기본적인 시간함수 : [util.h](https://github.com/parkseulkee/typing_game/blob/master/util.h) [util.c](https://github.com/parkseulkee/typing_game/blob/master/util.c)
* [main.c](https://github.com/parkseulkee/typing_game/blob/master/main.c)

 
