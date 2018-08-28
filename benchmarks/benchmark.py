#coding:utf8

import numpy as np

class Test(object):
  def __init__(self,max_n):
    self.text   = ""
    self.max_n  = max_n
    self.n      = 0
    self.rtol   = 0.0
    self.atol   = 0.0

  def value(self,name,test_value,ref_value):
    if not np.allclose(test_value,ref_value,rtol=self.rtol,atol=self.atol):
      self.n      += 1
      if self.max_is_reached():
        pass
      else:
        self.text   += "    %(n)s\n\tvalue %(v)s does not fit with reference %(r)s,"%{'n':name,'v':test_value,'r':ref_value}
        self.text   += " with atol=%(at)s and rtol=%(rt)s.\n"%{'at':self.atol,'rt':self.rtol}

  def vector(self,name,test_vector,ref_vector):
    for i,val in enumerate(test_vector):
      if not self.is_in_ref(val,ref_vector):
        self.n        += 1
        if self.max_is_reached():
          break
        else:
          self.text   += "    %(n)s\n\tvalue %(v)s not in the entries,"%{'n':name,'v':val}
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


class Data(object):
  """
  Class to extract and store simulation or input data from files.
  """
  def extract_G4(self,file_name,nthreads=2):
    """
    Extract data from a csv Geant4 output file.
    """
    data = []
    fext = 'csv'   # File extension
    fbase= file_name[:-5]   # File base name
    for thread in range(0,nthreads):
      fname=fbase+str(thread)+"."+fext
      with open(fname,'r') as f:
        for line in f.readlines():
          if line[0]!='#':
            for e in line.split(','):
              data.append(float(e))

    self.w   = np.array(data[0::8])
    self.x   = np.array(data[1::8])
    self.y   = np.array(data[2::8])
    self.z   = np.array(data[3::8])
    self.px  = np.array(data[4::8])
    self.py  = np.array(data[5::8])
    self.pz  = np.array(data[6::8])
    self.t   = np.array(data[7::8])

  def extract_txt(self,file_name):
    """
    Extract data from a text file
    """
    w,x,y,z,px,py,pz,t=np.loadtxt(file_name).T

    self.w   = w
    self.x   = x
    self.y   = y
    self.z   = z
    self.px  = px
    self.py  = py
    self.pz  = pz
    self.t   = t
