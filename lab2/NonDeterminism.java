//
// NonDeteminism.java
//
public class NonDeterminism {
    public static void main(String[] args) throws InterruptedException {
        class Container {
            public String value = "Empty";
        }
        final Container container = new Container();

        class FastThread extends Thread {
            public void run() {
                container.value = "Fast";
            }
        }

        class SlowThread extends Thread {

            FastThread fastThread;
            public SlowThread (FastThread ft) {
                this.fastThread = ft;
            }

            public void run() {
                try {
                    Thread.sleep(50);
                    this.fastThread.join();
                }
                catch(Exception e) {}
                container.value = "Slow";
            }
        }

        FastThread fast = new FastThread();
        SlowThread slow = new SlowThread(fast);
        fast.start(); slow.start();
        fast.join(); slow.join();
        System.out.println(container.value);
    }
}
