CR=$'\r\b'
# Int�r�t du \b sous Msys/Windows : �vite l'effet du -n dans echo qui �limine aussi le \r
for ((j=0;j<5;j++)) ; do
   echo -n $j 'Test'$CR
   sleep 2
done
