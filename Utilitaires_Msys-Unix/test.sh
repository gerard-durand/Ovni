CR=$'\r\b'
# Intérêt du \b sous Msys/Windows : évite l'effet du -n dans echo qui élimine aussi le \r
for ((j=0;j<5;j++)) ; do
   echo -n $j 'Test'$CR
   sleep 2
done
