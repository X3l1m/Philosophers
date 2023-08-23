# loop=20

# for((i=0; i < $loop; i++))
# do
# 	gcc mutex.c && ./a.out
# done

loop_count=15

# Hata durumunu takip edecek değişkeni başlatın
error_occurred=false

# Döngüyü kullanarak programı belirtilen sayıda çalıştırın
for ((i=1; i<=$loop_count; i++))
do
    echo -n "$i:	"
    gcc mutex.c && ./a.out
    exit_code=$?

    # Eğer program hata koduyla sonlandıysa, döngüyü kırın
    if [ $exit_code -ne 0 ]; then
        echo "Hata oluştu! Çalıştırmayı durduruyor..."
        error_occurred=true
        break
    fi
done

# Eğer hata oluştuysa, geri kalan çalıştırmaları atlayın
if $error_occurred; then
    remaining_runs=$((loop_count - i + 1))
    echo "Kalan çalıştırmalar ($remaining_runs adet) atlanacak."
fi