import java.util.Date;

/**
 * Created by Alexandr on 23.09.2017.
 */
public class MyClass {
    static {
        int a = 3;
        int b = a + 1;
        println("in MyClass static field");
    }

    public MyClass(){
        println("in MyClass constructor");
    }

    public static void main(String[] args) {
        System.getProperties().list(System.out);
        println("in main");
        String str = "aaa";
        println(str);
        changeString(str);
        println(str);
        ExperimentalObject eObject = new StaticClass();
        StaticClass staticClass = new StaticClass();
        println(staticClass.field);
        changeObject(staticClass);
        println(staticClass.field);
    }

    static void changeString(String str) {
        str = "bbb";
        println(str);
    }

    static void println(String str){
        System.out.println(str);
    }

    static void changeObject(Object obj){
        obj = new Date();
        println(obj.toString());
    }
}
