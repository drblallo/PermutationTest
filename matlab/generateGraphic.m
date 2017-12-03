function out = generateGraphic()
    name = [
        "mw_0.7_mean_gaussian_1000_out", ...
        "0.2_mean_uniform_1000_out", ...
        "ho_0.5_mean_gaussian_1000_4_vector_30_sample_out", ...
        "ho_same_mean_gaussian_1000_4_vector_30_sample_out", ...
        "mw_0.3_mean_uniform_1000_out", ...
        "mw_0-1_2-1_1-1_2-2_mixture_gaussian_1000_out", ...
        "mw_1_mean_chi_squared_1000_out", ...
        "mw_same_mean_chi_squared_1000_out", ...
        "mw_same_mean_gaussian_1000_out", ...
        "mw_same_mean_mixture_gaussian_1000_out", ...
        "mw_same_mean_uniform_1000_out", ...
        "same_mean_uniform_1000_out"];
    
    s = size(name);
    for t = 1:s(2)
        
        fileID = fopen(strcat('../c_output/', name(t), '.txt'), 'r+');
        a = fscanf(fileID, "%d %f\n");
        fclose(fileID);
        %out = a;
        out = a(1:2:size(a));
        
        figure('visible', 'off');
        s = scatter(a(1:2:size(a)), a(2:2:size(a)));
        xlabel('iterations');
        ylabel('change detected rateo');
        name2 = strrep(name(t), '_', ' ');
        title(name2);

        saveas(gcf, strcat('figures/', name(t), '.png'));
   end
end