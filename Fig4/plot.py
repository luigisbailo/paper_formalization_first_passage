import numpy as np
import matplotlib.pyplot as plt

plt.style.use(['style1'])
# f, ((ax1),(ax2)) = pl.subplots(2,1,sharex=True)

plt.figure(figsize=(3.37,2))
# f.set_figwidth(3.37)
# dataBM = np.loadtxt('plotBM.out')
# dataGF = np.loadtxt('plotGF_proj.out')

data = np.loadtxt('plot.out')
x = np.arange(0,1000,1)


plt.plot (x, 6*0.01*x,  '--', color='grey')
plt.plot (data[:,0], data[:,2],  'v',color='#377EB8', label = r'$p_{\Omega}(r,t|0,0)/S_{\Omega}(t|0,0)$')
plt.plot (data[:,0], data[:,1], '^', color='#B86637',label = r'$p_{\Omega}(r,t|0,0)$')
plt.plot (data[:,0], data[:,3],  'x', color='#B8A737',label = r'$BM$')

plt.ylabel (r'$MSD\thinspace [nm^2]$',fontsize=7)
plt.xlabel (r'$Time\thinspace [ns]$',fontsize=7)
plt.xticks ([0,250,500,750,1000])
plt.yticks ([0,20,40,60])

lgd=plt.legend (fontsize=7,ncol=2,bbox_to_anchor=(0.5, 1.4), loc='upper center', columnspacing=0)

plt.tight_layout()
#pl.subplots_adjust(top=0.6)
art = []
art.append(lgd)
# plt.show()
plt.savefig ('Fig4.eps',dpi=600, additional_artist=art, bbox_inches="tight")