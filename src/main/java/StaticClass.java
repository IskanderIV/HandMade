/**
 * Created by Alexandr on 24.09.2017.
 */
public class StaticClass implements ExperimentalObject{
    static protected int x = 3;
    static public int y = 5;
    public String field = "10";
    static {
        int a = 3;
        int b = a + 1;
        println("in StaticClass static field");
    }

    public StaticClass(){
        println("in StaticClass constructor");
    }

    static void println(String str){
        System.out.println(str);
    }
}
