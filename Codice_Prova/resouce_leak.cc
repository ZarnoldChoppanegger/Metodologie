void job()
    {
      Res* r1 = acquisisci_risorsa("res1");
      Res* r2 = acquisisci_risorsa("res2");
      do_task(r1,r2);
      rilascia_risorsa(r2);
      Res* r3 = acquisisci_risorsa("res3");
      do_task(r1,r3);
      rilascia_risorsa(r3);
      rilascia_risorsa(r1);
    }


1)

void job()
{
  Res* r1 = acquisisci_risorsa("res1");
  try{
    Res* = acquisisci_risorsa("res2");
    try{
      do_task(r1,r2);
      rilascia_risorsa(r2);
    }
    catch(...)
      {
	rilascia_risorsa(r2);
	throw;
      }
    Res* r3 = acquisisci_risorsa("res3");
    try{
      do_task(r1,r3);
      rilascia_risorsa(r3);
      rilascia_risorsa(r1);
    }
    catch(...)
      {
	rilascia_risorsa(r3);
	throw;
      }
  }
  catch(...)
    {
      rilascia_risorsa(r1);
      throw;
    }
}

2) soluzione



void job()
    {
      Res* r1 = acquisisci_risorsa("res1");
      Res* r2 = acquisisci_risorsa("res2");
      do_task(r1,r2);
      rilascia_risorsa(r2);
      Res* r3 = acquisisci_risorsa("res3");
      do_task(r1,r3);
      rilascia_risorsa(r3);
      rilascia_risorsa(r1);
    }


class RAI_RAID{
public:
  RAI_RAID(const char* nome ){
    r=acquisisci_risorsa(nome);
  }

  operator Res*() const{return r};

  RAI_RAID ()
  {rilascia_risorsa(r); }

private:
  Res* r;
  RAI_RAID(const RAI_RAID&)= delete;
  RAI_RAID& operator=(const RAI_RAID&)=delete;

}

  void job()
  {
    RAI_RAID r1("res1");
    {
      RAI_RAID r2("res2");
      do_task(r1,r2);
	}
    RAI_RAID r3("res3");
    do_task(r1,r3);
  }
