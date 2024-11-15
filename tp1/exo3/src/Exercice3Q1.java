public class Exercice3Q1 {

    // Classe interne statique ThreadInfo
    static class ThreadInfo {
        volatile long progress; // Stocke l'avancement d'un thread
        long result;            // Stocke le résultat d'un thread
    }

    // Tableau de ThreadInfo de taille 4
    static ThreadInfo[] infos = new ThreadInfo[4];

    // Fonction xorshf96() - générateur pseudo-aléatoire
    private static long x = 123456789, y = 362436069, z = 521288629; // Valeurs initiales

    public static long xorshf96() {
        long t;
        t = x ^ (x << 16);
        x = y;
        y = z;
        z = t ^ (t >> 5) ^ (z >> 1);
        return z;
    }

    public static void main(String[] args) throws InterruptedException {

        // Initialiser le tableau infos
        for (int i = 0; i < infos.length; i++) {
            infos[i] = new ThreadInfo();
        }

        Thread[] threads = new Thread[4];
        int iterations = 100_000_000; // 10^8 itérations

        // Timer
        long startTime = System.currentTimeMillis();
        // Créer et démarrer les threads
        for (int i = 0; i < threads.length; i++) {
            final int threadId = i;
            threads[i] = new Thread(() -> {
                for (int j = 0; j < iterations; j++) {
                    long value = xorshf96();
                    infos[threadId].result += value % 2; // Ajouter à result
                    infos[threadId].progress++;         // Incrémenter progress
                }
            });
            threads[i].start();
        }

        // Attendre la fin de tous les threads
        for (Thread thread : threads) {
            thread.join();
        }
        long endTime = System.currentTimeMillis();
        System.out.println("Time taken: " + (endTime - startTime) + "ms");

        // Afficher les résultats
        for (int i = 0; i < infos.length; i++) {
            System.out.println("Thread " + i + ": result = " + infos[i].result + ", progress = " + infos[i].progress);
        }
    }
}
