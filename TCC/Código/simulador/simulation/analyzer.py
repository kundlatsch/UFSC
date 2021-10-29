from statistics import mean


def get_mean():
    results = open("results.txt", "r")

    content = results.read().split(";")

    vtimes = []
    perceptions_processed = []
    plans_created = []

    for c in content:
        try:
            (vtime, perceptions, plans) = c.split(",")
            vtimes.append(float(vtime))
            perceptions_processed.append(float(perceptions))
            plans_created.append(int(plans))
        except ValueError:
            pass

    vt_mean = mean(vtimes)
    vt_string = "{:.2f}".format(vt_mean)
    print(f"VTIME: {vt_string}")

    pp_mean = mean(perceptions_processed)
    pp_string = "{:.2f}".format(pp_mean)
    print(f"PERCEPTIONS PROCESSED: {pp_string}")

    print(f"PLANS CREATED: {mean(plans_created)}")
