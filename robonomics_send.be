import json
import string

class RobonomicsSender
  var seconds_from_last_send
  var delay
  
  def init()
    self.delay = 10 * 60
    self.seconds_from_last_send = self.delay - 10
  end
    
  def every_second()
    self.seconds_from_last_send = self.seconds_from_last_send + 1
    if self.seconds_from_last_send > self.delay
      self.seconds_from_last_send = 0
      var energy_string = self.read_data()
      var tasmota_command = string.format("SendDatalog %s", energy_string)
      tasmota.cmd(tasmota_command)
    end
  end
  
  def read_data()
    var res = energy.read()
    var energy_data = {}
    energy_data["yesterday_sum"] = res.find("yesterday_sum")
    energy_data["daily"] = res.find("daily")
    energy_data["total"] = res.find("total")
    energy_data["voltage"] = res.find("voltage")
    energy_data["current"] = res.find("current")
    energy_data["active_power"] = res.find("active_power")
    var energy_string = json.dump(energy_data)
    return energy_string
  end
end

robonomics = RobonomicsSender()
tasmota.add_driver(robonomics)