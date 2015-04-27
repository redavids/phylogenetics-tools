import sys

if len(sys.argv) < 2:
        print 'usage python thisscript.py pathtoyourinputfile outfilename'

infilename = str(sys.argv[1])
 
outfilename = str(sys.argv[2])

qsubscriptname = 'WQMCquartetsfor.' + infilename[-40:n] +'.qsub'
 
headstr1 = '#declare name of job \n#PBS -N ruthquartets.' + infilename + '\n'

headstr2 = '# request 1 node \n#PBS -l nodes=1:ppn=1:taub \n# request 2 hours and 0 minutes of cpu time \n#PBS -l cput=02:00:00  \n# request 3 hours and 0 minutes of wall time (ask \n#PBS -l walltime=03:00:00  \n# mail is sent to you when the job starts and when it exits, terminates or aborts \n#PBS -m ea \n#this submits to queue cse \n#PBS -q cse \n#PBS -l naccesspolicy=singleuser \n# specify your email address \n#PBS -M redavids@ncsu.edu \n# By default, PBS scripts execute in your home directory, not the \n# directory from which they were submitted. The following line \n# places you in the directory from which the job was submitted.  \n# run the program; -t 1-2 runs it twice, 100 or 0-99 both run 100 times \n#PBS -t 1\n\n' 

cdstr = 'cd /home/redavid2/phylogenetics/quartets \n\n'

makequartetsstr = 'sh quartet-controller.sh ' + infilename + ' /home/redavid2/scratch/quartets/' + outfilename + '\n\n'

cdscratchstr = 'cd /home/redavid2/scratch/quartets \n\n'

wQMCstr = '/home/redavid2/scratch/quartets/' + outfilename

fixtoQMCstr = 'cat '+ wQMCstr +' | sed s/"(("//g | sed s/"),("/"|"/g | sed s/")); "/":"/g | sed \'/|/!d\' > ' +wQMCstr+'.wQMC\n\n'

endstr = 'done\n\nexit 0\n\n'


bigstring = headstr1 + headstr2 + cdstr + makequartetsstr + cdscratchstr + fixtoQMCstr + endstr 

H = open(qsubscriptname, 'w')

H.write(bigstring)

H.close()


