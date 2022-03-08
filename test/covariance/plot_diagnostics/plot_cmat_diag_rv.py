import os
import sys
import numpy as np
from netCDF4 import Dataset  # http://code.google.com/p/netcdf4-python/
import matplotlib.pyplot as plt
import argparse
import matplotlib

# Open file
DATADIR='../CMAT_00/'
f = Dataset(DATADIR + "/" + "mpas_parametersbump_cov_diag.nc", "r", format="NETCDF4")


print("f.groups=",f.groups)
print("----------------------------")
for group in f.groups:
   print("group=",group)
   print("----------------------------")
   print("f.groups[group].groups=",f.groups[group].groups)

   if( group == "surface_pressure-surface_pressure" ):
      break  # no vertical correlation for surface_pressure

   for subgroup in f.groups[group].groups:
      print("subgorup=",subgroup)
      print("----------------------------")
      raw=f.groups[group].groups[subgroup]["raw"][:]  #raw(nl0_1, nl0r, nc4, nc3)
      #print(type(raw))
      #print(raw.shape)
      fit=f.groups[group].groups[subgroup]["fit"][:]  #fit(nl0_1, nl0r, nc4, nc3)
      #print(type(fit))
      #print(fit.shape)
      l0rl0_to_l0=f.groups[group].groups[subgroup]["l0rl0_to_l0"][:]  # l0rl0_to_l0(nl0_1, nl0r)
      print(type(l0rl0_to_l0))
      print(l0rl0_to_l0.shape)
      #print(l0rl0_to_l0)
#1 #  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
#2 #  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
#3 #  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
#4 #  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
#5 #  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
#6 #  2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
#7 #  3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
#15  11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
#16  12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
#17  13, 14, 15, 16, 17, 18, 19, 20, 21, 22,

      for lev in range(55):   # python indice go with [0-54]
         var1=raw[lev,:,0,0]  ## NOTE here nc4=1
         var2=fit[lev,:,0,0]  ## NOTE here nc4=1
         #print(var1)
         #print(var2)

         fig, ax = plt.subplots()
         fig.subplots_adjust(right=0.8)
         ax.set_title(group+", lev="+str(lev+1))
         vlevels=l0rl0_to_l0[lev,:]
         ax.set_ylim([min(vlevels), max(vlevels)])
         ax.plot(var1, vlevels, label="raw")
         ax.plot(var2, vlevels, label="fit")
         handles, labels = ax.get_legend_handles_labels()
         fig.legend(handles, labels, loc='center right')
         plt.savefig("./diag_rv_"+group+"_"+str(lev+1).zfill(2)+".png", format="png", dpi=120)
         plt.close()
