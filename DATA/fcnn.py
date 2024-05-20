import random

def generate_connections(layer_sizes):
    connections = []
    node_indexs = []
    node_index = 0
    for layer_size in layer_sizes:
        node_in_layer = []
        for i in range(layer_size):
            node_in_layer.append(node_index)
            node_index += 1
        node_indexs.append(node_in_layer)

    for i in range(len(node_indexs)-1):
        for j in node_indexs[i]:
            for k in  node_indexs[i+1]:
                connections.append([j,k])


    return connections

def write_connections_to_file(connections, filename, layer_sizes):
    numnodes = 0
    for i in layer_sizes:
        numnodes += i
    with open(filename, 'w') as f:
        f.write(f"{numnodes} {len(connections)}\n")
        for connection in connections:
            f.write(f"{connection[0]} {connection[1]}\n")



if __name__ == "__main__":
    layer_sizes = [8,12,20,12,8]  # 每一层的节点数
    connections = generate_connections(layer_sizes)
    write_connections_to_file(connections, "network_connections60.txt", layer_sizes)
    # 打开输入文件
    with open("network_connections60.txt", "r+") as file:
        # 读取文件内容
        lines = file.readlines()

        # 从第二行开始，在每行末尾加一个空格和一个随机数
        for i in range(1, len(lines)):
            # 生成随机数
            random_number = random.randint(0, 100)
            
            # 在每行末尾加入空格和随机数
            lines[i] = lines[i].rstrip('\n') + ' ' + str(random_number) + '\n'

        # 将修改后的内容写回文件
        file.seek(0)
        file.writelines(lines)

    print("处理完成并写回到txt 中。")