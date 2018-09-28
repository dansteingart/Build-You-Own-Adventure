##pithytimeout=0 
from commands import getoutput as go
print go("pip install sseclient")
from sseclient import SSEClient
import json
import time
from pymongo import MongoClient as mc

mip = "172.17.0.2" ##Your MongoDB Address

client = mc(mip,27017)
#print client
cores = {}

ac = "" #->Get you token for this from Particle Build
get_event = "MAE_519_LAB_1"

while True:
    try:
        messages = SSEClient(
            'https://api.particle.io/v1/events/%s?access_token=%s' %(get_event,ac) 
            )
        for msg in messages:
            try:
                foo = msg.data.replace("nan","NaN")
                total = json.loads(foo)
                data = json.loads(total['data'])
                data['time'] = time.time()
                data['coreid'] = total['coreid']
                try:
                    data['tid'] = client[get_event][data['coreid']].find().count()+1
                    client[get_event][ data['coreid'] ].insert(data)
                except Exception as E:
                    print "try to insert:",E

                data['_id'] = data['coreid'] 
                _id = data['coreid'] 
                client[get_event]['status'].update({"_id":_id},{'$set':data},upsert=True)

            except Exception as E:
                print E,foo
                data = {'error':str(E),'time':time.time()}
                client[get_event+'_error'].errorlog.insert(data)

    except Exception as E:
        print "Data error: try",E
    time.sleep(5)




