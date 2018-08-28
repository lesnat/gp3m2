#coding:utf8

import numpy as np

def extract(file_name,nthreads=2):
  data = []
  fext = file_name.split('.')[-1]   # File extension
  fbase= file_name[:-(len(fext)+1)] # File base name

  for thread in range(0,nthreads):
    fname=fbase[:-1]+str(thread)+"."+fext
    with open(fname,'r') as f:
      for line in f.readlines():
        if line[0]!='#':
          for e in line.split(','):
            data.append(float(e))

  w   = np.array(data[0::7])
  x   = np.array(data[1::7])
  y   = np.array(data[2::7])
  z   = np.array(data[3::7])
  px  = np.array(data[4::7])
  py  = np.array(data[5::7])
  pz  = np.array(data[6::7])

  return w,x,y,z,px,py,pz

class Error(object):
  def __init__(self,max_n):
    self.text   = ""
    self.max_n  = max_n
    self.n      = 0
    self.rtol   = 0.0
    self.atol   = 0.0

  def test_value(self,name,test_value,ref_value):
    if not np.allclose(test_value,ref_value,rtol=self.rtol,atol=self.atol):
      self.n      += 1
      if self.max_is_reached():
        pass
      else:
        self.text   += "    %(n)s value %(v)s does not fit with reference %(r)s,"%{'n':name,'v':test_value,'r':ref_value}
        self.text   += " with atol=%(at)s and rtol=%(rt)s.\n"%{'at':self.atol,'rt':self.rtol}

  def test_vector(self,name,test_vector,ref_vector):
    for i,val in enumerate(test_vector):
      if not self.is_in_ref(val,ref_vector):
        self.n        += 1
        if self.max_is_reached():
          break
        else:
          self.text   += "    %(n)s value %(v)s not in the entries,"%{'n':name,'v':val}
          self.text   += " with atol=%(at)s and rtol=%(rt)s"%{'at':self.atol,'rt':self.rtol}
          self.text   += " (index = %(i)s).\n"%{'i':i}

  def max_is_reached(self):
    if self.n == self.max_n:
      self.text       += "    ...\n"
      self.text       += "    ...\n"
      self.text       += "    ..."
      return True
    elif self.n > self.max_n:
      return True
    else:
      return False

  def is_in_ref(self,test_value,ref_vector):
    is_in_ref=False
    for _,rval in enumerate(ref_vector):
      if np.allclose(rval,test_value,rtol=self.rtol,atol=self.atol):
        is_in_ref=True
        break
    return is_in_ref

  def print_results(self):
    SEPARATOR="\n###############################################################\n"
    print(SEPARATOR)
    if self.n != 0:
      print("Error(s) occured :\n")
      print(self.text)
    else:
      print("No error occured.")
    print(SEPARATOR)



class Benchmark(object):
    """
    Class to analyse gp3m2 benchmarks.

    Parameters
    ----------
    path : str
        path to the benchmark to analyse
    ref_file : str
        name of the reference file
    nthreads : int
        number of threads used in the simulation
    nevents : int
        number of events used in the simulation
    dpos : float
        diag position of the simulation
    max_error : int
        maximum number of errors to display
    """
    def __init__(self,path,ref_file,nthreads,nevents,dpos,max_error=10):
        # Simulation parameters
        self.path       = path
        self.ref_file   = ref_file
        self.nthreads   = nthreads
        self.nevents    = nevents
        self.dpos       = dpos
        # Errors
        self.text       = ""
        self.max_n      = max_error
        self.n          = 0
        self.rtol       = 0.0
        self.atol       = 0.0
        # Import results
        self.initialize()

    def initialize(self):
        """
        Import data
        """
        data = []
        fext = file_name.split('.')[-1]   # File extension
        fbase= file_name[:-(len(fext)+1)] # File base name

        for thread in range(0,nthreads):
            fname=fbase[:-1]+str(thread)+"."+fext
            with open(fname,'r') as f:
                for line in f.readlines():
                    if line[0]!='#':
                        for e in line.split(','):
                            data.append(float(e))

        sim = dict()
        self.sim["w"]   = np.array(data[0::8])
        self.sim["x"]   = np.array(data[1::8])
        self.sim["y"]   = np.array(data[2::8])
        self.sim["z"]   = np.array(data[3::8])
        self.sim["px"]  = np.array(data[4::8])
        self.sim["py"]  = np.array(data[5::8])
        self.sim["pz"]  = np.array(data[6::8])
        self.sim["pz"]  = np.array(data[6::8])
        self.sim["t"]   = np.array(data[7::8])

        ref = dict()


    def test_value(self,name,test_value,ref_value):
        if not np.allclose(test_value,ref_value,rtol=self.rtol,atol=self.atol):
            self.n      += 1
            if self.max_is_reached():
                pass
            else:
                self.text   += "    %(n)s value %(v)s does not fit with reference %(r)s,"%{'n':name,'v':test_value,'r':ref_value}
                self.text   += " with atol=%(at)s and rtol=%(rt)s.\n"%{'at':self.atol,'rt':self.rtol}

    def test_vector(self,name,test_vector,ref_vector):
        for i,val in enumerate(test_vector):
            if not self.is_in_ref(val,ref_vector):
                self.n        += 1
                if self.max_is_reached():
                    break
                else:
                    self.text   += "    %(n)s value %(v)s not in the entries,"%{'n':name,'v':val}
                    self.text   += " with atol=%(at)s and rtol=%(rt)s"%{'at':self.atol,'rt':self.rtol}
                    self.text   += " (index = %(i)s).\n"%{'i':i}

    def max_is_reached(self):
        if self.n == self.max_n:
            self.text       += "    ...\n"
            self.text       += "    ...\n"
            self.text       += "    ..."
            return True
        elif self.n > self.max_n:
            return True
        else:
            return False

    def is_in_ref(self,test_value,ref_vector):
        is_in_ref=False
        for _,rval in enumerate(ref_vector):
            if np.allclose(rval,test_value,rtol=self.rtol,atol=self.atol):
                is_in_ref=True
                break
        return is_in_ref

    def print_results(self):
        SEPARATOR="\n###############################################################\n"
        print(SEPARATOR)
        if self.n != 0:
            print("Error(s) occured :\n")
            print(self.text)
        else:
            print("No error occured.")
        print(SEPARATOR)


class _Sim(object):
    """
    Class to store simulation results
    """
    def __init__(self):
        pass


class _Ref(object):
    """
    Class to store reference results
    """
    def __init__(self):
        pass
