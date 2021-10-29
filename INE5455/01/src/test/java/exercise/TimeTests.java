package exercise;

import org.junit.jupiter.api.Test;
import org.joda.time.*;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class TimeTests {

	@Test
	void createTime() {
		//Fixture Setup
		
		//Exercise SUT
		LocalTime time = new LocalTime(1, 30, 20);
		
		//Result Verification
		assertEquals(01, time.getHourOfDay());
		assertEquals(30, time.getMinuteOfHour());
		assertEquals(20, time.getSecondOfMinute());
		
		//Fixture Teardown
	}

    @Test
	void createTimeInvalidHour() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(IllegalFieldValueException.class,() -> {
			LocalTime time = new LocalTime(25, 30, 20);
		});
        
        //Result Verification
		
		//Fixture Teardown
	}

    @Test
	void createTimeInvalidMinute() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(IllegalFieldValueException.class,() -> {
			LocalTime time = new LocalTime(1, 61, 20);
		});
        
        //Result Verification
		
		//Fixture Teardown
	}

    @Test
	void createTimeInvalidSecond() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(IllegalFieldValueException.class,() -> {
			LocalTime time = new LocalTime(1, 30, 100);
		});
        
        //Result Verification
		
		//Fixture Teardown
	}

    @Test
    void timeIsBefore() {
        //Fixture Setup
		LocalTime time1 = new LocalTime(12, 00, 00);
        LocalTime time2 = new LocalTime(18, 30, 00);

		//Exercise SUT
		Boolean before = time1.isBefore(time2);
		
		//Result Verification
		assertEquals(true, before);
		
		//Fixture Teardown
    }

    @Test
    void timeIsAfter() {
        //Fixture Setup
		LocalTime time1 = new LocalTime(12, 00, 00);
        LocalTime time2 = new LocalTime(06, 30, 00);

		//Exercise SUT
		Boolean after = time1.isAfter(time2);
		
		//Result Verification
		assertEquals(true, after);
		
		//Fixture Teardown
    }

    @Test
    void compareTwoTimes() {
		//Fixture Setup
		LocalTime time1 = new LocalTime(3, 50, 0);
        LocalTime time2 = new LocalTime(3, 20, 0);
		
		//Exercise SUT
		int compare = time1.compareTo(time2);
		
		//Result Verification
		assertEquals(1, compare);
		
		//Fixture Teardown
	}

    @Test
    void addHours() {
		//Fixture Setup
		LocalTime time = new LocalTime(21, 0, 0);
		
		//Exercise SUT
		LocalTime newTime = time.plusHours(4);
		
		//Result Verification
		assertEquals(1, newTime.getHourOfDay());
		
		//Fixture Teardown
	}


    @Test
    void addMinutes() {
		//Fixture Setup
		LocalTime time = new LocalTime(21, 0, 0);
		
		//Exercise SUT
		LocalTime newTime = time.plusMinutes(80);
		
		//Result Verification
		assertEquals(20, newTime.getMinuteOfHour());
		
		//Fixture Teardown
	}

    @Test
    void addSeconds() {
		//Fixture Setup
		LocalTime time = new LocalTime(21, 0, 0);
		
		//Exercise SUT
		LocalTime newTime = time.plusSeconds(130);
		
		//Result Verification
		assertEquals(10, newTime.getSecondOfMinute());
		
		//Fixture Teardown
	}

    @Test
    void subHours() {
		//Fixture Setup
		LocalTime time = new LocalTime(3, 0, 0);
		
		//Exercise SUT
		LocalTime newTime = time.minusHours(4);
		
		//Result Verification
		assertEquals(23, newTime.getHourOfDay());
		
		//Fixture Teardown
	}


    @Test
    void subMinutes() {
		//Fixture Setup
		LocalTime time = new LocalTime(3, 0, 0);
		
		//Exercise SUT
		LocalTime newTime = time.minusMinutes(80);
		
		//Result Verification
		assertEquals(40, newTime.getMinuteOfHour());
		
		//Fixture Teardown
	}

    @Test
    void subSeconds() {
		//Fixture Setup
		LocalTime time = new LocalTime(3, 0, 0);
		
		//Exercise SUT
		LocalTime newTime = time.minusSeconds(130);
		
		//Result Verification
		assertEquals(50, newTime.getSecondOfMinute());
		
		//Fixture Teardown
	}
}
