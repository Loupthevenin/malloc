#!/bin/bash

EXEC="./test_malloc"

# Menu interactif
echo -e "\n\033[1;35m=== Configuration Debug ===\033[0m"

read -r -p "🔧 Activer MALLOC_VERBOSE ? (y/n) " verbose
read -r -p "🧵 Activer MALLOC_TRACE ? (y/n) " trace
read -r -p "💣 Activer MALLOC_FAIL ? (y/n) " fail

echo -e "\n🛠️ Choisir un outil de détection mémoire/thread :"
echo "  [0] Aucun"
echo "  [1] Valgrind --leak-check=full"
echo "  [2] Valgrind --tool=massif"
echo "  [3] AddressSanitizer (ASAN)"
echo "  [4] ThreadSanitizer (TSAN)"
read -r -p "👉 Choix [0-4] : " tool_choice

# Flags
MALLOC_VERBOSE=0
MALLOC_TRACE=0
MALLOC_FAIL=0
VALGRIND_CMD=""
ASAN_MODE=0
TSAN_MODE=0

[[ "$verbose" == "y" ]] && MALLOC_VERBOSE=1
[[ "$trace" == "y" ]] && MALLOC_TRACE=1
[[ "$fail" == "y" ]] && MALLOC_FAIL=1

case "$tool_choice" in
1) VALGRIND_CMD="valgrind --leak-check=full" ;;
2) VALGRIND_CMD="valgrind --tool=massif" ;;
3) ASAN_MODE=1 ;;
4) TSAN_MODE=1 ;;
esac

# Compile le test
echo -e "\033[1;33m🔧 Compilation du test...\033[0m"

if [ "$ASAN_MODE" -eq 1 ]; then
	make asan
elif [ "$TSAN_MODE" -eq 1 ]; then
	make tsan
else
	make test
fi

if [ ! -f "$EXEC" ]; then
	echo -e "\033[1;31m❌ Erreur : exécutable $EXEC introuvable.\033[0m"
	exit 127
fi

# Résumé
echo -e "\n\033[1;34m🚀 Exécution avec la configuration suivante :\033[0m"
echo "MALLOC_VERBOSE=$MALLOC_VERBOSE"
echo "MALLOC_TRACE=$MALLOC_TRACE"
echo "MALLOC_FAIL=$MALLOC_FAIL"
[[ "$VALGRIND_CMD" != "" ]] && echo "Outils : $VALGRIND_CMD"
[[ "$ASAN_MODE" -eq 1 ]] && echo "Outil : AddressSanitizer"
[[ "$TSAN_MODE" -eq 1 ]] && echo "Outil : ThreadSanitizer"
echo ""

# Lancement
MALLOC_VERBOSE=$MALLOC_VERBOSE \
	MALLOC_TRACE=$MALLOC_TRACE \
	MALLOC_FAIL=$MALLOC_FAIL \
	LD_LIBRARY_PATH=. $VALGRIND_CMD $EXEC
