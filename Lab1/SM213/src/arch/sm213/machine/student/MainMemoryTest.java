package arch.sm213.machine.student;

import machine.AbstractMainMemory;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.omg.CORBA.DynAnyPackage.Invalid;

import static org.junit.Assert.*;

public class MainMemoryTest {
    private MainMemory mm;

    @Before
    public void setUp() {
        mm = new MainMemory(64);
    }

    @Test
    public void testIsAccessAligned() {
        // test when address is aligned
        assertTrue(mm.isAccessAligned(2,2));
        assertTrue(mm.isAccessAligned(4,2));
        assertTrue(mm.isAccessAligned(0,2));
        // test when address is not aligned
        assertFalse(mm.isAccessAligned(2,3));
        assertFalse(mm.isAccessAligned(3,2));

    }

    @Test
    public void testBytesToInteger() {
        // test 00000000 and convert to bytes 00 00 00 00
        assertEquals(0x00000000,mm.bytesToInteger((byte)00,(byte)00,(byte)00,(byte)00));
        // test 1A2B3C4D and convert to bytes 1A 2B 3C 4D
        assertEquals(0x1A2B3C4D,mm.bytesToInteger((byte)0x1A,(byte)0x2B,(byte)0x3C,(byte)0x4D));
        // test 5F3A2C8E and convert to bytes 5F 3A 2C 8E
        assertEquals(0x5F3A2C8E,mm.bytesToInteger((byte)0x5F,(byte)0x3A,(byte)0x2C,(byte)0x8E));
        // test FFFFFFFF and convert to bytes FF FF FF FF
        assertEquals(0xFFFFFFFF,mm.bytesToInteger((byte) 0xFF,(byte) 0xFF,(byte)0xFF,(byte)0xFF));
    }

    @Test
    public void testIntegerToBytes() {
        byte[] tb1;
        // test 00 00 00 00 and convert to integer 00000000
        tb1 = new byte[] {0x00,0x00,0x00,0x00};
        Assert.assertArrayEquals(tb1,mm.integerToBytes(0x00000000));
        // test FF FF FF FF and convert to integer FFFFFFFF
        tb1 = new byte[] {(byte)0xFF,(byte)0xFF,(byte)0xFF,(byte)0xFF};
        Assert.assertArrayEquals(tb1,mm.integerToBytes(0xFFFFFFFF));
        // test 5B 1A C9 E7 and convert to integer 5B1AC9E7
        tb1 = new byte[] {(byte) 0x5B,(byte) 0x1A,(byte) 0xC9,(byte) 0xE7};
        Assert.assertArrayEquals(tb1,mm.integerToBytes(0x5B1AC9E7));

    }
    @Test
    public void testGet() throws AbstractMainMemory.InvalidAddressException {

        try{
            mm.get(63,4);
        } catch (AbstractMainMemory.InvalidAddressException e){
            // expected
        }

        byte[] temp = new byte[]{1, 2, 3, 4};
        mm.set(0, temp);

        try{
            mm.get(0,4);
            Assert.assertArrayEquals(temp,mm.get(0,4));
        } catch (AbstractMainMemory.InvalidAddressException e){
            fail("error occured");
        }
    }
    @Test
    public void testSet() throws AbstractMainMemory.InvalidAddressException {

        byte[] temp = new byte[]{1, 2, 3};
        //when byte is not aligned
        try{
            mm.set(0,temp);
        } catch (AbstractMainMemory.InvalidAddressException e){
            // expected
        }

        byte[] temp2 = new byte[]{1, 2, 3, 4};
        mm.set(0, temp);

        // when memory is aligned
        try{
            mm.set(0,temp2);
            Assert.assertArrayEquals(temp2,mm.get(0,4));
        } catch (AbstractMainMemory.InvalidAddressException e){
            fail("error occured");
        }
    }
}
