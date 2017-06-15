class BasicProtocol {
private:
  /*...*/
public:
  BasicProtocol();
  ~BasicProtocol();
  bool BasicMsgA( /*...*/ );
  bool BasicMsgB( /*...*/ );
  bool BasicMsgC( /*...*/ );
};

class Protocol1 {
public:
  Protocol1();
  ~Protocol1();

  // i metodi chiameranno i metodi di
  // cui avranno bisogno, dall' oggetto bp_
  bool DoMsg1( /*...*/ );
  bool DoMsg2( /*...*/ );
  bool DoMsg3( /*...*/ );
private:
  BasicProtocol bp_;
};

class Protocol2 {
public:
  Protocol2();
  ~Protocol2();

  // i metodi chiameranno i metodi di
  // cui avranno bisogno, dall' oggetto bp_
  bool DoMsg1( /*...*/ );
  bool DoMsg2( /*...*/ );
  bool DoMsg3( /*...*/ );
  bool DoMsg4( /*...*/ );
  bool DoMsg5( /*...*/ );
private:
  BasicProtocol bp_;
};
