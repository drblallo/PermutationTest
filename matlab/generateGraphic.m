function out = generateGraphic()
    fileID = fopen('../c_output/mw_0.7_mean_gaussian_1000_out.txt', 'r+');
    a = fscanf(fileID, "%d %f\n");
    fclose(fileID);
    %out = a;
    out = a(1:2:size(a))
    scatter(a(1:2:size(a)), a(2:2:size(a)));
end