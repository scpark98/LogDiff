[동작지원]
- 1~n개까지 동시 비교가 가능하다.
  drag&drop하면 해당 문서를 열지만 ctrl키를 누르면 비교대상으로 추가된다.
- CRichEditCtrl을 이용한다.
- 스크롤은 모든 문서에 공통 적용된다.
- 시작 날짜/시간을 변경할 수 있다.
  해당 시각으로 시작 

[상태표시줄 정보]
- Notepad++과 동일하게 정보 표시 (Ln: %d    Col: %d    Pos: %,d    encoding)
- 

[m_content를 사용해야 하는지...]
- read() 함수로 통으로 읽어서 통으로 rich에 반영한다.
  이 때 라인수가 몇 만 라인인 파일은 다소 시간이 소용된다.
- m_content로 라인별로 읽고 rich를 clear한 후
  rich->add를 통해 다시 rich에 넣는다.
- arrange를 할 때는 rich의 각 라인을 하나의 list에 넣고
  timestamp의 차례로 sort한 후 다시 각 content에 넣고 이를 다시 rich에 표시한다.

* 몇 만 라인의 로그를 처리하려면 꽤 긴 시간이 소용되므로 이를 줄이고자 한다.
- read() 함수로 통으로 읽어서 통으로 rich에 반영한다.
- rich의 각 라인? 또는 통으로 읽어서 syntax coloring을 적용한다.
  thread 처리 필요.
- arrange 할 때는 list에 모든 rich의 라인들을 넣고 정렬한 후
  각 rich별로 string을 합친 후 rich에 다시 넣어준다.
- m_content를 사용하여 rich와 동기화하는 작업이 시간이 너무 소요되므로
  rich만을 사용하여 처리하는 방법으로 변경한다.