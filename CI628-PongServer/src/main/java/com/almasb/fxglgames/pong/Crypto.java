package com.almasb.fxglgames.pong;



import java.nio.charset.StandardCharsets;
import java.util.Random;

/**
 * @author Artemi Sementsenko (artemijsem) (artsem0107@gmail.com)
 */
public class Crypto  implements CryptoInterface{

    private char secretKey;

    @Override
    public String encryptDecrypt(String data_string) {

        String encoded_data = "";

        for (int i = 0; i < data_string.length(); i++)
        {
            // If number is even then add the key step if off then decrement the key step
            encoded_data = encoded_data + Character.toString((char)(data_string.charAt(i) ^ secretKey));
        }


        return encoded_data;
    }

    @Override
    public void generateKey() {

        Random r = new Random();
        secretKey = (char)(r.nextInt(26) + 'a');
    }

    @Override
    public int getSecretKey() {
        return secretKey;
    }
}
