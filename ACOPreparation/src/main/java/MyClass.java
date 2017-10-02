import java.util.Date;

import static java.lang.System.out;

/**
 * Created by Alexandr on 23.09.2017.
 *
 */
public class MyClass {
    static {
        int a = 3;
        int b = a + 1;
        println("in MyClass static field");
    }

    public MyClass() {
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
        NonStaticObject nonStaticObject1 = new NonStaticObject();
        NonStaticObject nonStaticObject2 = new NonStaticObject();
        nonStaticObject1.b = 5;
        nonStaticObject2 = nonStaticObject1;
        println(nonStaticObject2);
        println(nonStaticObject1);

        nonStaticObject1.b = 7;
        println(nonStaticObject2);
        println(nonStaticObject1);
    }

    private static void changeString(String str) {
        str = "bbb";
        println(str);
    }

    private static void println(Object str) {
        out.println(str);
    }
    static void print(Object str) {
        out.print(str);
    }

    static void changeObject(Object obj) {
        obj = new Date();
        println(obj.toString());
    }
}
