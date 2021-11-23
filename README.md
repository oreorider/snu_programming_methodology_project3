# snu_programming_methodology_project3


# FAQ

- Q) 음식을 더 못 넣는 상황에서 txt파일에 있는 음식들을 전부 안넣는건가요? 아니면 넣을 수 있을때까지는 넣어주나요?
  - A) 넣을 수 있을 때 까지 넣어줍니다.
- Q) 못 넣은 음식을 따로 한꺼번에 출력해주는 형식인가요?
  - A) 매 번 insert를 실패할 때 마다 출력해주면 됩니다.
- Q) pop을 할 때 exp date이 같은 음식이 여러개 있을 때 어떤 음식을 pop하나요?
  - A) exp가 같은 음식이 여러개이면 오른쪽 위 음식부터 pop해줍니다.
- Q) SR에서 위쪽 선반 음식을 모두 pop하는 경우 그 선반을 지워주나요?
  - A) 네
- Q) pop을 했을 때 왼쪽으로 음식을 밀어서 빈공간을 채우는 것은 각각의 shelf에서만 수행하는 건가요? 예를들면 1번 shelf 빈공간에 2번 shelf 음식을 내려서 채울 수 있는지 궁금합니다.
  - A) 불가능합니다. pop이 된 선반에서만 음식의 x좌표 위치 변화가 일어나고 타 선반 음식의 x좌표는 변하지 않습니다.
- Q) meal recommendation 동점
  - A) 랜덤하게 출력하셔도 됩니다.
- Q) 단순히 meal에 사용된 재료가 적어서 남은 재료들이 많아서 exp date 합이 높은 경우도 score가 높은 것으로 치는 건가요?
  - A) 네. 
- Q) 같은 재료 다른 유통기한이면, 유통기한이 짧은 것부터 사용을 하는 것인가요?
  - A) 네.
- Q) 재료의 만족도나 유통기한은 모두 정수인가요?
  - A) 네.
- Q) smart controller에서 음식이나 shelf를 옮겨주는 것은 어디에서 하는 기능인가요?
  - A) stackFood / popFood 에서 합니다.
- Q) 함수 파라미터나 리턴타입은 수정 가능한가요?
  - A) 불가능합니다.
- Q) 레포트 모든 내용을 영어로 작성해야 하나요?
  - A) 네
- Q) including pseudo codes 가 어떤 의미인가요?
  - A) https://en.wikipedia.org/wiki/Pseudocode
- Q) 
- Q) 혼자 해도 되나요?
  - A) 네
- Q) 
  - A) 
  
# 정정
- p23. position to pop: 12 -> x:12, y:0 (x, y 둘 다 출력해야 합니다.)
- p34. 남은 음식 
