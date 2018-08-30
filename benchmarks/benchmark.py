#coding:utf8

import numpy as _np

class Test(object):
  """
  Class to test values, collect and print errors.

  Parameters
  ----------
  nerrors : int
    maximum number of errors to print for each test
  atol : float
    absolute tolerance
  rtol : float
    relative tolerance
  """
  def __init__(self,nerrors,atol=0.0,rtol=0.0):
    self.text   = ""
    self.nerrors= nerrors
    self.error  = False
    self.rtol   = rtol
    self.atol   = atol
    self.SEPARATOR="\n###############################################################\n"

  def print_init(self,id):
    """
    Print a start message for the benchmarking.

    Parameters
    ----------
    id : str
      benchmark id
    """
    print(self.SEPARATOR)
    print("Starting benchmark %s analysis ..."%id)
    print(self.SEPARATOR)

  def print_result(self,label,n):
    """
    Print the result of a given test.

    Parameters
    ----------
    label : str
      name of the test
    n : int
      number of time the test failed
    """
    if n>0:
      print(label+" ... ERROR ...")
    else:
      print(label+" ... OK !")

  def print_errors(self):
    """
    Print all the test errors.
    """
    print(self.SEPARATOR)
    if self.error:
      print("Error(s) occured :\n")
      print(self.text)
    else:
      print("No error occured (atol={a}, rtol={r})."
            .format(a=self.atol,r=self.rtol))
    print(self.SEPARATOR)

  def add_error(self,text):
    """
    Add an error to the buffer.

    Parameters
    ----------
    text : str
      text to add
    """
    self.error = True
    self.text += text

  def max_is_reached(self,n):
    """
    Test if the maximum number of errors is reached for a given test.

    Parameters
    ----------
    n : int
      number of time the test failed
    """
    if n == self.nerrors + 1:
      self.text       += "\t...\n"
      return True
    elif n > self.nerrors:
      return True
    else:
      return False

  def is_in_ref(self,tval,rvect):
    """
    Test if the value tval is present in the vector rvect.

    Parameters
    ----------
    tval : float or int
      test value
    rvect : list or np.array
      reference vector
    """
    is_in_ref=False
    for _,rval in enumerate(rvect):
      if _np.allclose(rval,tval,rtol=self.rtol,atol=self.atol):
        is_in_ref=True
        break
    return is_in_ref

  def value(self,label,tval,rval):
    """
    Test if the tval and rval values are approximately the same.

    Parameters
    ----------
    label : str
      name of the test
    tval : float or int
      test value
    rval : float or int
      reference value
    """
    n = 0
    if not _np.allclose(tval,rval,rtol=self.rtol,atol=self.atol):
      n += 1
      self.add_error(
      """\t{label} :\n\t  value {tval: .4E} is not equal to reference {rval: .4E}, with atol={atol:.4E} and rtol={rtol:.4E}.\n"""
      .format(label=label,tval=tval,rval=rval,atol=self.atol,rtol=self.rtol)
      )

    self.print_result(label,n)

  def vector(self,label,tvect,rvect,both=True):
    """
    Test if the elements of tvect are in rvect.

    Parameters
    ----------
    label : str
      name of the test
    tvect : list or np.array
      test vector
    rvect : list or np.array
      reference vector
    both : bool, optional
      if True also test if elements of rvect are in tvect. Default is True
    """
    # Set current label, derived from given label
    if both:
      clabel = label  + " (test in ref)"
    else:
      clabel = label

    # Test in ref
    n = 0
    for i,val in enumerate(tvect):
      if not self.is_in_ref(val,rvect):
        n += 1
        if self.max_is_reached(n):
          break
        else:
          self.add_error(
          """\t{label} :\n\t  value {val: .4E} is not present in the reference, with atol={atol:.2E} and rtol={rtol:.2E} (index = {i}).\n"""
          .format(label=clabel,val=val,atol=self.atol,rtol=self.rtol,i=i)
          )
    self.print_result(clabel,n)
    # Ref in test
    if both:
      self.vector(label+" (ref in test)",rvect,tvect,both=False)


class Data(object):
  """
  Class to extract and store simulation or input data from files.

  Parameters
  ----------
  specie : str
    label of the specie. Available are e-,e+,gamma
  """
  def __init__(self,specie):
    self.specie = specie
    if specie in ("e-","e+","electron","positron"):
      self.mass = 0.511
    elif specie in ("gamma"):
      self.mass = 0
    else:
      raise NameError("Unknown specie label")

  def extract_G4(self,file_name,nthreads=2):
    """
    Extract data from a csv Geant4 output file.
    """
    data = []
    fext = 'csv'   # File extension
    fbase= file_name[:-5]   # File base label
    for thread in range(0,nthreads):
      fname=fbase+str(thread)+"."+fext
      with open(fname,'r') as f:
        for line in f.readlines():
          if line[0]!='#':
            for e in line.split(','):
              data.append(float(e))

    self.w   = _np.array(data[0::8])
    self.x   = _np.array(data[1::8])
    self.y   = _np.array(data[2::8])
    self.z   = _np.array(data[3::8])
    self.px  = _np.array(data[4::8])
    self.py  = _np.array(data[5::8])
    self.pz  = _np.array(data[6::8])
    self.t   = _np.array(data[7::8])

  def extract_txt(self,file_name):
    """
    Extract data from a text file
    """
    w,x,y,z,px,py,pz,t=_np.loadtxt(file_name).T

    self.w   = w
    self.x   = x
    self.y   = y
    self.z   = z
    self.px  = px
    self.py  = py
    self.pz  = pz
    self.t   = t

  def r(self):
    return _np.sqrt(self.y**2 + self.z**2)

  def d(self,x0=0,y0=0,z0=0):
    return _np.sqrt((self.x-x0)**2 + (self.y-y0)**2 + (self.z-z0)**2)

  def v(self):
    return self.beta() * 2.99792458e8 * 1e6 / 1e15

  def p(self):
    return _np.sqrt(self.px**2 + self.py**2 + self.pz**2)

  def ekin(self):
    if self.mass==0:
      return self.p()
    else:
      return (_np.sqrt((self.p()/self.mass)**2 + 1) - 1) * self.mass

  def gamma(self):
    if self.mass==0:
      return self.ekin() # FIXME : vérifier
    else:
      return self.ekin()/self.mass + 1.

  def beta(self):
    if self.mass==0:
      return _np.array([1]*len(self.w))
    else:
      beta =[]
      for g in self.gamma():
        if g>0:
          beta.append(_np.sqrt(1 - (1/g)**2))
        else:
          beta.append(0)
      return _np.array(beta)
