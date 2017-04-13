void foo() {
  A* a1 = new A(1);
  A* a2 = new A(2);
  try {
    job1(a1, a2);
    job2(a1, new A(3));
  } catch (...) {
    delete a2;
    delete a1;
  }
}

void foo(){
  //metti try per ogni risorsa acquisita
  A* a1 = new A(1);
  try{ //guardia a1
    A* a2 = new A(2);
    try{ //guardia a2
      job1(a1, a2);
      A* a3 = new A(3);
      try{ //guardia a3
	job2(a1, a3);
	delete a3;
      }
      catch(...){
	delete a3;
	throw;
      }
      delete a2;
      delete a1;
      catch(...){
	delete a2;
	throw;
      }
    }
  }

  catch(...){
    delete a1;
  }
}
