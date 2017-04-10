class Date{
public:
  Date(int dd, int mm, int yy);

private:
  int d, m, y;
};

Date::Date(int dd, int mm, int yy)
{
  d = dd;
  m = mm;
  y = yy;
}

int main(){
  Date oggi = Date(10,04,2017); //OK
  Date today{10, 04, 2017}; //OK
  Date tomorrow(11, 04, 2017); //abbreviazione
  Date toyota; //ERRORE, no inizializzazione, quindi no chiamata a costruttore

  return 0;
}
