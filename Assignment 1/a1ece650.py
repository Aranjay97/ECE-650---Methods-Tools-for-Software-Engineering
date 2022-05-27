#!/usr/bin/env python3
import re
import sys
import math
import ast

### Add Road Function

def addroad(a_s,street,str_co,c_m_dict):
  a_s.append(street)
  ns = [] #new street
  if (street in c_m_dict.keys()):
    print("Error: Street already exists")
    return 
  for i in range(0,len(str_co)-1):
    x = str_co[i]
    y = str_co[i+1]
    if x[-1] == ')' and y[-1] == ')' and x[0] == '(' and y[0] == '(':
      ns.append(x + '-->' + y)
    else:
       print("Error: Enter all coordinates within brackets")
  c_m_dict[street] = ns
  return c_m_dict

### Modify Road Function

def modroad(a_s,street,street_co,c_m_dict):
  ns = [] #new street
  if (street not in c_m_dict.keys()):
    print("Error: Street doesnt exist")
    return
  for i in range(0,len(street_co)-1):
    x = street_co[i]
    y = street_co[i+1]
    if x[-1] == ')' and y[-1] == ')' and x[0] == '(' and y[0] == '(':
      ns.append(x + '-->' + y)
    else:
      print("Error: Enter all coordinates within brackets")
  c_m_dict[street] = ns
  return c_m_dict

### Remove Road Function

def rmroad(a_s,street,c_m_dict):
  if (street not in c_m_dict.keys()):
    print("Error: Street doesnt exist")
    return
  del c_m_dict[street]
  a_s.remove(street)

### GG command Function

def ggroad(a_s,c_m_dict):
  # Find vertices
  interseccords,lineeqn,interseccord_keys = cal_vtx(vertices,vdict,a_s,c_m_dict)
  # Find edges
  edges = cal_edges(interseccords,vdict,vertices,lineeqn)
  print("V = {")
  for k,v in vertices.items():
    vr = "(" + "{0:.2f}".format(v[0]) + "," + "{0:.2f}".format(v[1]) + ")"
    print("  {}: {}".format(k, vr))
  print("}")
  
  print("E = {")
  count=0
  for e in edges:
    count += 1
    if count < len(edges):
      print("  "+ e + ',')
    else:
      print("  "+ e)
  print("}")
  vdict.clear()
  interseccords.clear()
  
### Find intersection function

def findint(x1,x2,x3,x4,y1,y2,y3,y4):
  def line(p1, p2):
    A = (p1[1] - p2[1])
    B = (p2[0] - p1[0])
    C = (p1[0]*p2[1] - p2[0]*p1[1])
    return A, B, -C
  def intersection(L1, L2):
    D  = L1[0] * L2[1] - L1[1] * L2[0]
    Dx = L1[2] * L2[1] - L1[1] * L2[2]
    Dy = L1[0] * L2[2] - L1[2] * L2[0]
    if D != 0:
        x = Dx / D
        y = Dy / D
        return x,y
    else:
        return False
  l1 = line([x1,y1],[x2,y2])
  l2 = line([x3,y3],[x4,y4])
  if intersection(l1,l2) == False:
    return False
  else:
    a,b = intersection(l1,l2)
    if a<=max(x1,x2) and a>=min(x1,x2) and a<=max(x3,x4) and a>=min(x3,x4) and b<=max(y1,y2) and b>=min(y1,y2) and b<=max(y3,y4) and b>=min(y3,y4):
      return a,b
    else:
      return False


def point_exists_InLine(x1,y1,x2,y2,x3,y3,x4,y4,vpointdist):

	if not (x4 - x3) == 0 and not (x2 - x1) == 0:
		slope1 = (y2 - y1) / (x2 - x1)
		slope2 = (y4-y3) / (x4-x3)
		if slope1 == slope2:

			Intercept1 = y1 - (slope1 * x1)
			Intercept2 = y3 - (slope2 * x3)

				##Check if X1,Y1 lies in Equation2
			Interceptcheck = y1 - (slope2 * x1)
			if Interceptcheck == Intercept2:
			
				# if (x2-x1) == 0 or  (x4-x3) == 0:
				if min(x1,x2) <= min(x3,x4) and max(x1,x2) >= max(x3,x4) and min(y1,y2) <= min(y3,y4) and max(y1,y2) >= max(y3,y4):

						vpointdist.append(x3)
						vpointdist.append(y3)
						vpointdist.append(x4)
						vpointdist.append(y4)
				elif min(x3,x4) <= min(x1,y1) and max(x3,x4) >= max(x1,x2) and min(y3,y4) <= min(y1,y2) and max(y3,y4) >= max(y1,y2):

						vpointdist.append(x1)
						vpointdist.append(y1)
						vpointdist.append(x2)
						vpointdist.append(y2)
	else:

		if x1 == x2 == x3 == x4:
			if min(x1,x2) <= min(x3,x4) and max(x1,x2) >= max(x3,x4) and min(y1,y2) <= min(y3,y4) and max(y1,y2) >= max(y3,y4):

					vpointdist.append(x3)
					vpointdist.append(y3)
					vpointdist.append(x4)
					vpointdist.append(y4)
			elif min(x3,x4) <= min(x1,y1) and max(x3,x4) >= max(x1,x2) and min(y3,y4) <= min(y1,y2) and max(y3,y4) >= max(y1,y2):

					vpointdist.append(x1)
					vpointdist.append(y1)
					vpointdist.append(x2)
					vpointdist.append(y2)
	return (vpointdist)

### function to calculate and keep vertices in same order after removing

def verticesOrder(vertices,vkeys,vdict,vertices1):
	if not vdict == {}:
		vertices.clear()
		ikey = 1
		for key in vkeys:
			vertices[ikey] =ast.literal_eval(key)
			ikey =ikey + 1
		for value in vdict.values():
			for item in range(0,len(value)):
				if not ast.literal_eval(value[item]) in vertices.values():
					vertices[ikey] =  ast.literal_eval(value[item])
					ikey = ikey + 1	
		ikey = 1
		for key in vkeys:
			vertices1[ikey] = ast.literal_eval(key)
			ikey =ikey + 1
		for value in vdict.values():
			for item in range(0,len(value)):
				if ast.literal_eval(value[item]) not in vertices1.values():
					vertices1[ikey] = ast.literal_eval(value[item])
					ikey = ikey + 1
		vertices = vertices1.copy()	
		return vertices

		for keyold,oldvalue in vertices.items():
			flag = 0
			for newvalue in vertices1.items():
				if oldvalue == newvalue[1]:
					break
				else:
					flag = flag + 1
					if flag == len(vertices1):
						del vertices[keyold]
		
		if vertices == {}:

			for key,newvalue1 in vertices1.items():
				vertices[key]= newvalue1

		else:
			for newvalue1 in vertices1.items():
				flag = 0
				for oldvalue1 in vertices.items():
					if oldvalue1[1] == newvalue1[1]:
						break
					else:
						flag = flag + 1
						if flag == len(vertices):
							keyv = oldvalue1[0] + 1
							vertices[keyv] = newvalue1[1]


	else:
		vertices.clear()

### function to store intersections and line segments

def mildict(base,compar,interseccords,interseccord_keys,lineeqn,V):
	l1 = base
	l2 = compar	
	temp = []							
	if l1 not in lineeqn:
		lineeqn.append(l1)
	if l2 not in lineeqn:
		lineeqn.append(l2)

	if V not in interseccord_keys:
		interseccord_keys.append(V)
		temp.append(l1)
		temp.append(l2)
		interseccords[V]= temp[:]
	else:
		if not l1 in interseccords[V]:
			interseccords[V].append(l1)	
		if not l2 in interseccords[V]:
			interseccords[V].append(l2)

### Function to calculate vertices


def cal_vtx(vertices,vdict,a_s,c_m_idct):
  vlist = []
  vkeys = []
  vertices1 = {}
  interseccords = {}
  lineeqn = []
  interseccord_keys = []
  for i in range(0,len(a_s)-1):
    key = a_s[i]
    for base in c_m_dict[key]:
      j = i+1
      line1 = re.findall(r'(-?\d+\.?\d*)',base) #base line to compare
      for k in range(j,len(a_s)): 
        for compar in c_m_dict[a_s[k]]:  
          line2 = re.findall(r'(-?\d+\.?\d*)',compar) 
          x1, y1 = float(line1[0]), float(line1[1])
          x2, y2 = float(line1[2]), float(line1[3])
          x3, y3 = float(line2[0]), float(line2[1])
          x4, y4 = float(line2[2]), float(line2[3])
          if x1 == x3 and y1 == y3:
            V = '(' + str(x1) + ',' + str(y1) +')'
            if V not in vkeys:
              vkeys.append(V)			
              vlist.append('(' + str(x2) + ',' + str(y2) +')')
              vlist.append('(' + str(x4) + ',' + str(y4) +')')				
              vdict[V] = vlist[:]		
              del vlist[:]	
              mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
            else:
              tempcoord = '(' + str(x2) + ',' + str(y2) +')'
              if tempcoord not in vdict[V]:
                vdict[V].append(tempcoord)		
              tempcoord = '(' + str(x4) + ',' + str(y4) +')'
              if tempcoord not in vdict[V]:
                vdict[V].append(tempcoord)
              mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)

          elif x1 == x4 and y1 == y4:
            V = '(' + str(x1) + ',' + str(y1) +')'
            if V not in vkeys:
              vkeys.append(V)			
              vlist.append('(' + str(x2) + ',' + str(y2) +')')
              vlist.append('(' + str(x3) + ',' + str(y3) +')')				
              vdict[V] = vlist[:]		
              del vlist[:]	
              mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
            else:
              tempcoord = '(' + str(x2) + ',' + str(y2) +')'
              if tempcoord not in vdict[V]:
                vdict[V].append(tempcoord)		
              tempcoord = '(' + str(x3) + ',' + str(y3) +')'
              if tempcoord not in vdict[V]:
                vdict[V].append(tempcoord)
              mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)

          elif x2 == x4 and y2 == y4:
            V = '(' + str(x2) + ',' + str(y2) +')'
            if V not in vkeys:
              vkeys.append(V)			
              vlist.append('(' + str(x1) + ',' + str(y1) +')')
              vlist.append('(' + str(x3) + ',' + str(y3) +')')				
              vdict[V] = vlist[:]		
              del vlist[:]
              mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)	
            else:
              tempcoord = '(' + str(x1) + ',' + str(y1) +')'
              if tempcoord not in vdict[V]:
                vdict[V].append(tempcoord)		
              tempcoord = '(' + str(x3) + ',' + str(y3) +')'
              if tempcoord not in vdict[V]:
                vdict[V].append(tempcoord)
              mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
          
          elif x2 == x3 and y2 == y3:
            V = '(' + str(x2) + ',' + str(y2) +')'
            if V not in vkeys:
              vkeys.append(V)			
              vlist.append('(' + str(x1) + ',' + str(y1) +')')
              vlist.append('(' + str(x4) + ',' + str(y4) +')')				
              vdict[V] = vlist[:]		
              del vlist[:]	
              mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
            else:
              tempcoord = '(' + str(x1) + ',' + str(y1) +')'
              if tempcoord not in vdict[V]:
                vdict[V].append(tempcoord)		
              tempcoord = '(' + str(x4) + ',' + str(y4) +')'
              if tempcoord not in vdict[V]:
                vdict[V].append(tempcoord)
              mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
          else:
            if(findint(x1,x2,x3,x4,y1,y2,y3,y4) == False):
              vpointdist = []
              i = 0
              vpointdist = point_exists_InLine(x1,y1,x2,y2,x3,y3,x4,y4,vpointdist)
              if not vpointdist == []:
                for eindex in range(0,len(vpointdist)-1,2):	
                  V = '(' + str(vpointdist[eindex]) + ',' + str(vpointdist[eindex+1]) +')'
                  if V not in vkeys:
                    vkeys.append(V)
                    vlist.append('(' + str(x1) + ',' + str(y1) +')')
                    vlist.append('(' + str(x2) + ',' + str(y2) +')')
                    vlist.append('(' + str(x3) + ',' + str(y3) +')')
                    vlist.append('(' + str(x4) + ',' + str(y4) +')')
                    vdict[V] = vlist[:]
                    del vlist[:]
                    mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
              
                  else:
                    tempcoord = '(' + str(x1) + ',' + str(y1) +')'
                    if tempcoord not in vdict[V]:
                      vdict[V].append(tempcoord)
                    tempcoord = '(' + str(x2) + ',' + str(y2) +')'
                    if tempcoord not in vdict[V]:
                      vdict[V].append(tempcoord)
                    tempcoord = '(' + str(x3) + ',' + str(y3) +')'
                    if tempcoord not in vdict[V]:
                      vdict[V].append(tempcoord)
                    tempcoord = '(' + str(x4) + ',' + str(y4) +')'
                    if tempcoord not in vdict[V]:
                      vdict[V].append(tempcoord)
                    mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
            else:
              xcoor,ycoor = findint(x1,x2,x3,x4,y1,y2,y3,y4)
              V = '(' + str(xcoor) + ',' + str(ycoor) +')'
              if V not in vkeys:
                vkeys.append(V)
                vlist.append('(' + str(x1) + ',' + str(y1) +')')
                vlist.append('(' + str(x2) + ',' + str(y2) +')')
                vlist.append('(' + str(x3) + ',' + str(y3) +')')
                vlist.append('(' + str(x4) + ',' + str(y4) +')')
                vdict[V] = vlist[:]
                del vlist[:]
                mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
              
              else:
                tempcoord = '(' + str(x1) + ',' + str(y1) +')'
                if tempcoord not in vdict[V]:
                  vdict[V].append(tempcoord)
                tempcoord = '(' + str(x2) + ',' + str(y2) +')'
                if tempcoord not in vdict[V]:
                  vdict[V].append(tempcoord)
                tempcoord = '(' + str(x3) + ',' + str(y3) +')'
                if tempcoord not in vdict[V]:
                  vdict[V].append(tempcoord)
                tempcoord = '(' + str(x4) + ',' + str(y4) +')'
                if tempcoord not in vdict[V]:
                  vdict[V].append(tempcoord)	
                mildict(base,compar,interseccords,interseccord_keys,lineeqn,V)
  verticesOrder(vertices,vkeys,vdict,vertices1)
  return (interseccords,lineeqn,interseccord_keys)



### Function to calculate edges

def cal_edges(interseccords,vdict,vertices,lineeqn):
	intersecs = []
	iodistance = []
	edges = []
	for line in lineeqn:
		for k,v in interseccords.items():
			if line in v:
				if k not in intersecs:
					intersecs.append(k)
		a1,a2,a3,a4 = re.findall(r'(-?[+-]?\d*\.?\d+)',line)
		if len(intersecs) == 1:
			ck1 = '('+ a1 + ',' + a2 + ')'
			ck2 = '('+ a3 + ',' + a4 + ')'
			ck1 = ast.literal_eval(ck1)
			ck2 = ast.literal_eval(ck2)
			if not str(list(vertices.keys())[list(vertices.values()).index(ast.literal_eval(intersecs[0]))]) == str( list(vertices.keys())[list(vertices.values()).index(ck1)]):
				e1 = '<' + str(list(vertices.keys())[list(vertices.values()).index(ast.literal_eval(intersecs[0]))]) + ',' + str( list(vertices.keys())[list(vertices.values()).index(ck1)]) + '>'
				if e1 not in edges:edges.append(e1)
			if not str(list(vertices.keys())[list(vertices.values()).index(ast.literal_eval(intersecs[0]))]) == str( list(vertices.keys())[list(vertices.values()).index(ck2)]):
				e1 = '<' + str(list(vertices.keys())[list(vertices.values()).index(ast.literal_eval(intersecs[0]))]) + ',' + str( list(vertices.keys())[list(vertices.values()).index(ck2)]) + '>'
				if e1 not in edges: edges.append(e1)
			del intersecs[:]
			ck1,ck2 = '',''
		
		if len(intersecs) > 2:

			pointointdistance=[]
			endpointdistance = []
			for echi in range(0,len(intersecs)):
				c1,c2 = re.findall(r'(-?[+-]?\d*\.?\d+)',intersecs[echi])
				pidist = math.sqrt( (float(c2) - float(a2))**2 + (float(c1) - float(a1))**2 )
				pointointdistance.append(pidist)
				endpidist = math.sqrt( (float(c2) - float(a4))**2 + (float(c1) - float(a3))**2 )
				endpointdistance.append(endpidist)
				
			f1,f2 = re.findall(r'(-?[+-]?\d*\.?\d+)',(intersecs[pointointdistance.index(min(pointointdistance))]))
			g1,g2 = re.findall(r'(-?[+-]?\d*\.?\d+)',(intersecs[endpointdistance.index(min(endpointdistance))]))
			newintersecs = intersecs[:]
			
			ck1 = '('+ f1 + ',' + f2 + ')'	
			ck2 = '('+ a1 + ',' + a2 + ')'
			reference = ck1
			newintersecs.remove(ck1)
			ck1 = ast.literal_eval(ck1)
			ck2 = ast.literal_eval(ck2)

			if not str(list(vertices.keys())[list(vertices.values()).index(ck1)]) == str(list(vertices.keys())[list(vertices.values()).index(ck2)]):
				e1 = '<' + str(list(vertices.keys())[list(vertices.values()).index(ck1)]) +  ',' + str( list(vertices.keys())[list(vertices.values()).index(ck2)]) + '>'
				if e1 not in edges: edges.append(e1)

			ck1 = ''
			ck2 = ''

			ck1 = '('+ g1 + ',' + g2 + ')'	
			ck2 = '('+ a3 + ',' + a4 + ')'
		
			ck1 = ast.literal_eval(ck1)
			ck2 = ast.literal_eval(ck2)

			if not str(list(vertices.keys())[list(vertices.values()).index(ck1)]) == str( list(vertices.keys())[list(vertices.values()).index(ck2)]):
				e1 = '<' + str(list(vertices.keys())[list(vertices.values()).index(ck1)]) + ','+ str( list(vertices.keys())[list(vertices.values()).index(ck2)]) + '>'
				if e1 not in edges: edges.append(e1)

			ck1 = ''
			ck2 = ''

			for inte in newintersecs[:]:
				# if not len(newintersecs) == 1:
				for ke in range(0,len(newintersecs)):
					b1,b2 = re.findall(r'(-?[+-]?\d*\.?\d+)',reference)			
					b3,b4 = re.findall(r'(-?[+-]?\d*\.?\d+)',newintersecs[ke])
					dist = math.sqrt ( (float(b4) - float(b2))**2 + (float(b3) - float(b1))**2 )
					iodistance.append(dist)
				if not iodistance == []	:
					kindex = iodistance.index(min(iodistance))
					b = re.findall(r'(-?[+-]?\d*\.?\d+)',(newintersecs[kindex]))
					b3 = b[0]
					b4 = b[1]
					ck1 = reference	
					ck2 = '('+ b3 + ',' + b4 + ')'
					ck1 = ast.literal_eval(ck1)
					ck2 = ast.literal_eval(ck2)
					if not str(list(vertices.keys())[list(vertices.values()).index(ck1)]) == str( list(vertices.keys())[list(vertices.values()).index(ck2)]):
						e1 = '<' + str(list(vertices.keys())[list(vertices.values()).index(ck1)]) + ',' + str( list(vertices.keys())[list(vertices.values()).index(ck2)]) + '>'
						if e1 not in edges:edges.append(e1)
					reference = '('+ b3 + ',' + b4 + ')'
					newintersecs.pop(kindex)
					ck1,ck2 = '',''
					iodistance = []
				
				else:
					break


		elif len(intersecs) == 2:
					twodistance = []
					c1,c2 = re.findall(r'(-?[+-]?\d*\.?\d+)',intersecs[0])
					twodist = math.sqrt( (float(c2) - float(a2))**2 + (float(c1) - float(a1))**2 )
					twodistance.append(twodist)
					twodist = math.sqrt( (float(c2) - float(a4))**2 + (float(c1) - float(a3))**2 )
					twodistance.append(twodist)

					if twodistance.index(min(twodistance)) == 0:
						fr1  = ast.literal_eval(intersecs[0])
						end1 = ast.literal_eval(intersecs[1])
					else:
						fr1  = ast.literal_eval(intersecs[1])
						end1 = ast.literal_eval(intersecs[0])
					ck1 = '('+ a1 + ',' + a2 + ')'
					ck2 = '('+ a3 + ',' + a4 + ')'
					ck1 = ast.literal_eval(ck1)
					ck2 = ast.literal_eval(ck2)
					
					if not str(list(vertices.keys())[list(vertices.values()).index(fr1)]) == str(list(vertices.keys())[list(vertices.values()).index(ck1)]):
						e1 = '<' + str(list(vertices.keys())[list(vertices.values()).index(fr1)]) + ',' + str( list(vertices.keys())[list(vertices.values()).index(ck1)]) + '>'
						if e1 not in edges: edges.append(e1)
					if not str(list(vertices.keys())[list(vertices.values()).index(end1)]) == str(list(vertices.keys())[list(vertices.values()).index(ck2)]):
						e1 = '<' + str(list(vertices.keys())[list(vertices.values()).index(end1)]) + ',' + str(list(vertices.keys())[list(vertices.values()).index(ck2)]) + '>'
						if e1 not in edges: edges.append(e1)

					i1 = ast.literal_eval(intersecs[0])
					i2 = ast.literal_eval(intersecs[1])
					if not str(list(vertices.keys())[list(vertices.values()).index(i1)]) == str(list(vertices.keys())[list(vertices.values()).index(i2)]):
						e1 = '<' + str(list(vertices.keys())[list(vertices.values()).index(i1)]) + ',' + str(list(vertices.keys())[list(vertices.values()).index(i2)]) + '>'
						if e1 not in edges: edges.append(e1)
		intersecs = []

	return(edges)
      

### Main Function

def main():

  while(True):
    s = sys.stdin.readline()
    s = s.replace('\n','')
    if s == '':
      break
    if not s.startswith('add') and not s.startswith('mod') and not s.startswith('gg') and not s.startswith('rm'):
      print("Error: invalid function command")
      continue
    elif (s == "gg"):
      if len((a_s)) == 0:
        print("V = {")
        print("}")
        print("E = {")
        print("}") 
      else:
        ggroad(a_s,c_m_dict)
    else:
      d = s.split(" ",1)
      if(len(d)<2):
        print("Error: Insufficient Data")
        continue
      splitup1 = d[0].split('"',1)
      if len(splitup1) > 1 and not s.startswith('r') == 'True':
        print("Error: Missing Space between function and street name")
        continue
      splitup2 = d[1].split('"(',1)
      if len(splitup2) > 1 and not s.startswith('r') == 'True':
        print("Error: Missing Space between street name and coordinates")
        continue
      street_temp = d[1].split('"',2)
      street = street_temp[1].lower()
      st = street_temp[2]
      if not re.match(r'[a-zA-Z\s]*$',street) or street == " " or not re.match(r'\S[a-zA-Z\s]*\S$',street):
        print("Error:No special character,number,empty string or leading spaces allowed in street name")
        continue
      splitup3 = street_temp[2].split(')(',1)
      if len(splitup3) > 1 and not s.startswith('r') == 'True':
        print("Error: Missing Space between brackets in co-ordinates")
        continue

      street_temp[2] = street_temp[2].replace(" ",'')
      street_temp[2] = street_temp[2].replace("\n",'')
      rawcoord = len(street_temp[2])
      check_pair_coord =re.findall('\s*[-]*[0-9]+\.?[0-9]*\s*',street_temp[2])
      if not len(check_pair_coord) % 2 == 0:
        print("Error: Missing Pair X,Y of Coordinate values")
        continue
      parsed_coord = re.findall(r'\(.*?\)',street_temp[2])
      op_c = street_temp[2].count('(')
      cp_c = street_temp[2].count(')')
    
      if not op_c == cp_c:
        print("Error: Missing Open or Closing brackets")
        continue
      street_co = parsed_coord #Street co-ordinates
      if len(street_co)<2 and not s.startswith('rm'):
        print("Error : Invalid command")
        continue
      f=0
      for i in street_temp[2]:
        if i == '+':
          f = 1
          print("Error: + sign not allowed in coordinates")
          continue
      if f==1:
        continue
      for i in range(0,len(st)):
        if st[i] == '-' and st[i+1] == ' ':
          f = 1
          print("Error: whitespace not allowed after negative sign in coordinates")
          break
      if f == 1:
        continue
      f=0
      for i in range(0,len(st)):
        if (st[i] == '(' and st[i+1] == ' ') or (st[i] == ' ' and st[i+1] == ')') or (st[i] == ' ' and st[i+1] == ',') or (st[i] == ',' and st[i+1] == ' ') :
          f = 1
          print("Error: whitespace not allowed anywhere within parenthesis in specifying coordinates")
          break
      if f == 1:
        continue 

         
      if (s.startswith('add')):
        addroad(a_s,street,street_co,c_m_dict)
      elif (s.startswith('mod')):
        modroad(a_s,street,street_co,c_m_dict)
      elif (s.startswith('rm')):
        if len(street_co)>2:
          print("Error : Invalid command,cannot have 2 arguments")
          continue
        rmroad(a_s,street,c_m_dict)


    # return exit code 0 on successful termination
  sys.exit(0)

if __name__ == "__main__":
  ### declaring variables

  a_s = [] #to store names of all streets 
  c_m_dict  = {}
  lines = []
  edges = []
  vertices = {}
  vdict = {}
  interseccords = {}
  interseccord_keys = []
  lineeqn = []
  V = ''
  main()



